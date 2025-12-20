#!/usr/bin/env bash
set -euo pipefail

gcc -Wall -Wextra -O2 -std=c11 -Include -o server server.c
gcc -Wall -Wextra -O2 -std=c11 -Iinclude -o client \
	src/client_main.c src/client_args.c src/ipc_client.c src/record.c src/scanner.c src/util.c

echo "[OK] build ./server ./client"


