//go:build windows

package main

import (
	"fmt"
	"strings"
	"syscall"
	"unsafe"
)

const interactive = true

var (
	comdlg32         = syscall.NewLazyDLL("comdlg32.dll")
	user32           = syscall.NewLazyDLL("user32.dll")
	pGetOpenFileName = comdlg32.NewProc("GetOpenFileNameW")
	pMessageBox      = user32.NewProc("MessageBoxW")
)

// OPENFILENAMEW (commdlg.h).
type openFileName struct {
	lStructSize       uint32
	hwndOwner         uintptr
	hInstance         uintptr
	lpstrFilter       *uint16
	lpstrCustomFilter *uint16
	nMaxCustFilter    uint32
	nFilterIndex      uint32
	lpstrFile         *uint16
	nMaxFile          uint32
	lpstrFileTitle    *uint16
	nMaxFileTitle     uint32
	lpstrInitialDir   *uint16
	lpstrTitle        *uint16
	flags             uint32
	nFileOffset       uint16
	nFileExtension    uint16
	lpstrDefExt       *uint16
	lCustData         uintptr
	lpfnHook          uintptr
	lpTemplateName    *uint16
	pvReserved        uintptr
	dwReserved        uint32
	flagsEx           uint32
}

func pick(title string) string {
	filter := syscall.StringToUTF16("ROM de MAME (*.zip)\x00*.zip\x00Todos\x00*.*\x00\x00")
	buf := make([]uint16, 4096)
	t, _ := syscall.UTF16PtrFromString(title)
	ofn := openFileName{
		lpstrFilter: &filter[0],
		lpstrFile:   &buf[0],
		nMaxFile:    uint32(len(buf)),
		lpstrTitle:  t,
		flags:       0x00001000 | 0x00000800 | 0x00080000, // FILEMUSTEXIST|PATHMUSTEXIST|EXPLORER
	}
	ofn.lStructSize = uint32(unsafe.Sizeof(ofn))
	r, _, _ := pGetOpenFileName.Call(uintptr(unsafe.Pointer(&ofn)))
	if r == 0 {
		return ""
	}
	return syscall.UTF16ToString(buf)
}

func askFiles(have []string) []string {
	hasName := func(n string) bool {
		for _, h := range have {
			if strings.EqualFold(lastName(h), n) {
				return true
			}
		}
		return false
	}
	if !hasName("mslugx.zip") {
		fmt.Println("Elige la ROM de MAME: mslugx.zip")
		if p := pick("Elige mslugx.zip (ROM de MAME)"); p != "" {
			have = append(have, p)
		}
	}
	if !hasName("neogeo.zip") {
		fmt.Println("Elige la BIOS de MAME: neogeo.zip")
		if p := pick("Elige neogeo.zip (BIOS de Neo Geo)"); p != "" {
			have = append(have, p)
		}
	}
	return have
}

func lastName(p string) string {
	if i := strings.LastIndexAny(p, `\/`); i >= 0 {
		return p[i+1:]
	}
	return p
}

func notify(ok bool, msg string) {
	title := "Metal Slug X64"
	flags := uintptr(0x40) // MB_ICONINFORMATION
	if !ok {
		flags = 0x10 // MB_ICONERROR
	}
	m, _ := syscall.UTF16PtrFromString(msg)
	t, _ := syscall.UTF16PtrFromString(title)
	pMessageBox.Call(0, uintptr(unsafe.Pointer(m)), uintptr(unsafe.Pointer(t)), flags)
}
