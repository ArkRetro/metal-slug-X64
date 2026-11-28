//go:build !windows

package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

const interactive = false

func askFiles(have []string) []string {
	r := bufio.NewReader(os.Stdin)
	for len(have) < 2 {
		fmt.Print("Ruta del zip (mslugx.zip o neogeo.zip), vacío para terminar: ")
		s, _ := r.ReadString('\n')
		s = strings.Trim(strings.TrimSpace(s), "\"")
		if s == "" {
			break
		}
		have = append(have, s)
	}
	return have
}

func notify(ok bool, msg string) {}
