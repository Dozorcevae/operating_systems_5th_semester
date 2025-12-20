#!/usr/bin/env bash
set -euo pipefail

ROOT="${1:-$PWD/tests/test_fs}"

echo "[+] Creating test FS at: $ROOT"
rm -rf "$ROOT"
mkdir -p "$ROOT"

# normal directory + file
mkdir -p "$ROOT/ok_dir"
echo "hello" > "$ROOT/ok_dir/file.txt"

# directory without access (should produce {!})
mkdir -p "$ROOT/no_access"
chmod 000 "$ROOT/no_access"

# world-writable file (should produce {*})
echo "world writable" > "$ROOT/world_writable.txt"
chmod 666 "$ROOT/world_writable.txt"

# symlink (should produce {S} and not recurse)
ln -s "$ROOT/ok_dir" "$ROOT/link_to_ok"

# nested structure to show depth
mkdir -p "$ROOT/ok_dir/sub1/sub2"
touch "$ROOT/ok_dir/sub1/sub2/deep.txt"

echo "[OK] Test FS created."
echo "Hint: after testing you can cleanup with: chmod 755 \"$ROOT/no_access\" && rm -rf \"$ROOT\""
