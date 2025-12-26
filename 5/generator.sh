#!/bin/bash
DATA_FILE="data.txt"

touch "$DATA_FILE" || { echo "Error: cannot create $DATA_FILE"; exit 1; }

while true; do
  IFS= read -r LINE || exit 0

  printf '%s\n' "$LINE" >> "$DATA_FILE" || { echo "Error: write failed"; exit 1; }

  if [[ "$LINE" == "QUIT" ]]; then
    exit 0
  fi
done