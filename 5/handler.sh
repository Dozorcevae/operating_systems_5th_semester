#!/bin/bash
DATA_FILE="data.txt"

touch "$DATA_FILE" || { echo "Error: cannot create $DATA_FILE"; exit 1; }

cleanup() {
  if [[ -n "${TAIL_PID:-}" ]]; then
    kill "$TAIL_PID" 2>/dev/null
    wait "$TAIL_PID" 2>/dev/null
  fi
}
trap cleanup EXIT INT TERM

# "Слежение" за файлом: tail -n 0 -f
coproc TAILPROC { tail -n 0 -f "$DATA_FILE"; }
TAIL_PID="$TAILPROC_PID"

# Читаем вывод tail построчно (как read в МУ через конвейер)
while IFS= read -r LINE <&"${TAILPROC[0]}"; do
  echo "$LINE"

  if [[ "$LINE" == "QUIT" ]]; then
    echo "QUIT received. Handler stops."
    exit 0
  fi
done
