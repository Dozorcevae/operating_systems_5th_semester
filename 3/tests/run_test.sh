#!/usr/bin/env bash
set -euo pipefail

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ROOT_FS="${PROJECT_DIR}/tests/test_fs"
OUT_LOG="${PROJECT_DIR}/tests/server_output.txt"
SOCK="/tmp/lab_3_audit_tree.sock"

cd "$PROJECT_DIR"

# 0) prepare test data
./tests/setup_test_fs.sh "$ROOT_FS"

# 1) ensure stale socket removed
rm -f "$SOCK"

# 2) start server in background, redirect output to file
echo "[+] Starting server..."
./server > "$OUT_LOG" 2>&1 &
SERVER_PID=$!
sleep 0.2

# 3) run client against test FS
echo "[+] Running client..."
./client --root "$ROOT_FS" --max-depth 10 --max-nodes 5000

# 4) give server a moment to flush
sleep 0.2

# 5) stop server (if still alive)
if kill -0 "$SERVER_PID" 2>/dev/null; then
  kill "$SERVER_PID" 2>/dev/null || true
fi

echo "[OK] Done."
echo "Server output saved to: $OUT_LOG"
echo "Open it with: less \"$OUT_LOG\""
