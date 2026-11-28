package main

import (
	"bytes"
	"compress/flate"
	"crypto/aes"
	"crypto/cipher"
	"crypto/rand"
	"crypto/sha256"
	"encoding/binary"
	"errors"
	"fmt"
	"os"
)

// packMain (developer only): -pack ROM.z64 OUT.bin mslugx.zip neogeo.zip
// Encrypts a verified cartridge with the key of the original files.
func packMain(args []string) error {
	if len(args) < 4 {
		return errors.New("uso: -pack ROM.z64 payload.bin mslugx.zip neogeo.zip")
	}
	rom, err := os.ReadFile(args[0])
	if err != nil {
		return err
	}
	found, err := collect(args[2:])
	if err != nil {
		return err
	}
	for i, f := range found {
		if f == nil {
			return fmt.Errorf("falta %s", required[i].name)
		}
	}
	var comp bytes.Buffer
	w, _ := flate.NewWriter(&comp, flate.BestCompression)
	w.Write(rom)
	w.Close()
	block, _ := aes.NewCipher(deriveKey(found))
	gcm, _ := cipher.NewGCM(block)
	sum := sha256.Sum256(rom)
	var out bytes.Buffer
	out.WriteString(magic)
	binary.Write(&out, binary.LittleEndian, uint64(len(rom)))
	out.Write(sum[:])
	prefix := make([]byte, 4)
	rand.Read(prefix)
	out.Write(prefix)
	data := comp.Bytes()
	for idx := uint64(0); len(data) > 0; idx++ {
		n := min(chunkSize, len(data))
		nonce, ad := chunkNonce(prefix, idx, n == len(data))
		sealed := gcm.Seal(nil, nonce, data[:n], ad)
		binary.Write(&out, binary.LittleEndian, uint32(len(sealed)))
		out.Write(sealed)
		data = data[n:]
	}
	fmt.Printf("rom %d bytes, sha256 %x, payload %d bytes\n", len(rom), sum, out.Len())
	return os.WriteFile(args[1], out.Bytes(), 0o644)
}
