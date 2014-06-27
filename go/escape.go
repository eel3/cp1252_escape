/* ********************************************************************** */
/**
 * @brief   Command line tool to escape characters greater than 0x7F.
 * @author  eel3
 * @date    2013/01/22
 *
 * @par Compilers
 * - Go 1.3.0 windows/amd64
 */
/* ********************************************************************** */

package main

import (
	"flag"
	"fmt"
	"io"
	"os"
	"path"
)

const (
	exit_success = iota // EXIT_SUCCESS
	exit_failure        // EXIT_FAILURE
)

// Initialize function (specify help message).
func init() {
	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, "usage: %s [-o out-file] [file ...]\n", path.Base(os.Args[0]))
		flag.PrintDefaults()
	}
}

// Escape characters greater than 0x7F.
func doEscape(in *os.File, out *os.File) {
	buf := make([]byte, 1)
	for {
		n, err := in.Read(buf)
		if n == 0 && err == io.EOF {
			break
		}
		if buf[0] >= 0x80 {
			fmt.Fprintf(out, `\x%X`, buf[0])
		} else {
			out.Write(buf)
		}
	}
}

// Read file at `infile' and escape all text.
func escapeFile(infile string, out *os.File) error {
	var in *os.File

	if infile == "-" {
		in = os.Stdin
	} else {
		var err error
		in, err = os.Open(infile)
		if err != nil {
			return err
		}
		defer in.Close()
	}

	doEscape(in, out)
	return nil
}

// Main routine.
func main() {
	outfile := flag.String("o", "-", "output file or `-' (stdout)")
	flag.Parse()

	var out *os.File
	if *outfile == "-" {
		out = os.Stdout
	} else {
		var err error
		out, err = os.OpenFile(*outfile, os.O_CREATE|os.O_WRONLY, 0666)
		if err != nil {
			fmt.Println(err)
			os.Exit(exit_failure)
		}
		defer out.Close()
	}

	retval := exit_success

	if flag.NArg() <= 0 {
		doEscape(os.Stdin, out)
	} else {
		for i := 0; i < flag.NArg(); i++ {
			err := escapeFile(flag.Arg(i), out)
			if err != nil {
				fmt.Println(err)
				retval = exit_failure
			}
		}
	}

	os.Exit(retval)
}
