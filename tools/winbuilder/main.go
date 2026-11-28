// Metal Slug X64 builder: rebuilds metalslugx64_rev1.0.z64 from the user's
// own MAME 0.264 sets (mslugx.zip and neogeo.zip).
//
// The cartridge data is shipped only as an encrypted payload (payload.bin,
// embedded at build time, never in git). The key is the SHA-256 of the
// contents of the original ROM files below, so the payload opens only with
// the exact MAME dumps; the published SHA-1 hashes identify the files but do
// not give the key. The result is checked against the SHA-256 of ROM 53.
//
// Standard library only: GOOS=windows GOARCH=amd64 go build gives one .exe
// with no runtime dependencies.
package main

import (
	"archive/zip"
	"bufio"
	"bytes"
	"compress/flate"
	"crypto/aes"
	"crypto/cipher"
	"crypto/sha1"
	"crypto/sha256"
	_ "embed"
	"encoding/binary"
	"encoding/hex"
	"errors"
	"fmt"
	"hash/crc32"
	"io"
	"os"
	"path/filepath"
	"strings"
)

const outName = "metalslugx64_rev1.0.z64"
const keyLabel = "metalslugx64 rev1.0 payload key v1"
const magic = "MSX64PK1"
const chunkSize = 4 << 20

// required lists the MAME 0.264 files the cartridge is derived from, in key
// order. Files are matched by content (CRC-32, then SHA-1), not by name, so
// any correctly dumped set works whatever its zip layout.
type romFile struct {
	set, name string
	size      int
	crc       uint32
	sha1      string
}

//go:embed payload.bin
var payload []byte

func main() {
	if len(os.Args) > 1 && os.Args[1] == "-pack" {
		if err := packMain(os.Args[2:]); err != nil {
			fmt.Fprintln(os.Stderr, "error:", err)
			os.Exit(1)
		}
		return
	}
	err := run(os.Args[1:])
	if err != nil {
		fmt.Println()
		fmt.Println("ERROR:", err)
		notify(false, err.Error())
	}
	pause()
	if err != nil {
		os.Exit(1)
	}
}

func run(args []string) error {
	fmt.Println("Metal Slug X64 - generador de", outName)
	fmt.Println("Necesita tus ficheros de MAME 0.264: mslugx.zip y neogeo.zip")
	fmt.Println()
	inputs := args
	if len(inputs) == 0 {
		inputs = findNextToExe()
	}
	if len(inputs) < 2 {
		inputs = askFiles(inputs)
	}
	if len(inputs) == 0 {
		return errors.New("no se ha elegido ningún fichero")
	}
	found, err := collect(inputs)
	if err != nil {
		return err
	}
	var missing []string
	for i, r := range required {
		if found[i] == nil {
			missing = append(missing, r.set+".zip: "+r.name)
		}
	}
	if len(missing) > 0 {
		return fmt.Errorf("faltan ficheros (o no son el volcado correcto de MAME):\n  %s", strings.Join(missing, "\n  "))
	}
	fmt.Println("Ficheros originales comprobados (", len(required), ").")
	key := deriveKey(found)
	fmt.Println("Descifrando y montando la ROM...")
	rom, err := openPayload(payload, key)
	if err != nil {
		return err
	}
	dir := filepath.Dir(inputs[0])
	out := filepath.Join(dir, outName)
	if err := os.WriteFile(out, rom, 0o644); err != nil {
		return err
	}
	fmt.Println()
	fmt.Println("Listo:", out)
	fmt.Printf("Tamaño %d bytes, SHA-256 comprobado.\n", len(rom))
	notify(true, "ROM creada:\n"+out)
	return nil
}

func findNextToExe() []string {
	exe, err := os.Executable()
	if err != nil {
		return nil
	}
	dir := filepath.Dir(exe)
	var out []string
	for _, n := range []string{"mslugx.zip", "neogeo.zip"} {
		p := filepath.Join(dir, n)
		if _, err := os.Stat(p); err == nil {
			out = append(out, p)
		}
	}
	return out
}

// collect reads every entry of every given zip and keeps the contents of the
// required files, matched by CRC-32 and confirmed by SHA-1.
func collect(paths []string) ([][]byte, error) {
	found := make([][]byte, len(required))
	for _, p := range paths {
		z, err := zip.OpenReader(p)
		if err != nil {
			return nil, fmt.Errorf("%s: no es un zip válido (%v)", filepath.Base(p), err)
		}
		for _, f := range z.File {
			for i, r := range required {
				if found[i] != nil || f.CRC32 != r.crc || f.UncompressedSize64 != uint64(r.size) {
					continue
				}
				rc, err := f.Open()
				if err != nil {
					z.Close()
					return nil, err
				}
				data, err := io.ReadAll(rc)
				rc.Close()
				if err != nil {
					z.Close()
					return nil, fmt.Errorf("%s: %s dañado (%v)", filepath.Base(p), f.Name, err)
				}
				s := sha1.Sum(data)
				if crc32.ChecksumIEEE(data) == r.crc && hex.EncodeToString(s[:]) == r.sha1 {
					found[i] = data
				}
			}
		}
		z.Close()
	}
	return found, nil
}

func deriveKey(found [][]byte) []byte {
	h := sha256.New()
	h.Write([]byte(keyLabel))
	for _, d := range found {
		var n [8]byte
		binary.LittleEndian.PutUint64(n[:], uint64(len(d)))
		h.Write(n[:])
		h.Write(d)
	}
	return h.Sum(nil)
}

// Payload: magic | size u64 | sha256(rom) | nonce prefix [4] | chunks.
// Each chunk: length u32 | AES-256-GCM(deflate stream part), nonce =
// prefix | index u64, additional data = index u64 | last flag.
func openPayload(p []byte, key []byte) ([]byte, error) {
	if len(p) < 52 || string(p[:8]) != magic {
		return nil, errors.New("el programa está incompleto (falta el paquete de datos)")
	}
	size := binary.LittleEndian.Uint64(p[8:16])
	want := p[16:48]
	prefix := p[48:52]
	block, _ := aes.NewCipher(key)
	gcm, _ := cipher.NewGCM(block)
	var comp bytes.Buffer
	rest := p[52:]
	for idx := uint64(0); len(rest) > 0; idx++ {
		if len(rest) < 4 {
			return nil, errors.New("paquete de datos dañado")
		}
		n := binary.LittleEndian.Uint32(rest[:4])
		if uint64(len(rest)-4) < uint64(n) {
			return nil, errors.New("paquete de datos dañado")
		}
		chunk := rest[4 : 4+n]
		rest = rest[4+n:]
		nonce, ad := chunkNonce(prefix, idx, len(rest) == 0)
		plain, err := gcm.Open(nil, nonce, chunk, ad)
		if err != nil {
			return nil, errors.New("los ficheros de MAME no abren el paquete (¿son de MAME 0.264?)")
		}
		comp.Write(plain)
	}
	rom, err := io.ReadAll(flate.NewReader(&comp))
	if err != nil {
		return nil, fmt.Errorf("paquete de datos dañado (%v)", err)
	}
	got := sha256.Sum256(rom)
	if uint64(len(rom)) != size || !bytes.Equal(got[:], want) {
		return nil, errors.New("la ROM generada no coincide con la original (SHA-256)")
	}
	return rom, nil
}

func chunkNonce(prefix []byte, idx uint64, last bool) (nonce, ad []byte) {
	nonce = make([]byte, 12)
	copy(nonce, prefix)
	binary.LittleEndian.PutUint64(nonce[4:], idx)
	ad = make([]byte, 9)
	binary.LittleEndian.PutUint64(ad, idx)
	if last {
		ad[8] = 1
	}
	return
}

func pause() {
	if !interactive {
		return
	}
	fmt.Println()
	fmt.Print("Pulsa Intro para salir...")
	bufio.NewReader(os.Stdin).ReadString('\n')
}
