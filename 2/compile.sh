#!/bin/bash
set -u

echo "Compiling C programs..."

build() {
	src="$1"
	out="$2"
	gcc -Wall -Wextra -O2 -o "$out" "$src"
	if [ $? -eq 0 ]; then
		echo "[OK] $src -> $out"
	else
		echo "[ERROR] Failed: $src"
		exit 1;
	fi
}

build hello_world.c hello_world
build help_program.c help_program
build read_text.c read_text
build number_lines.c number_lines
build print_names.c print_names

echo "Done."

