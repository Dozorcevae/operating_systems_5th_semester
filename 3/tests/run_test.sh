#!/usr/bin/env bash
set -euo pipefail

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ROOT_FS="${PROJECT_DIR}/tests/test_fs"
OUT_LOG="${PROJECT_DIR}/tests/server_output.txt"
SOCK="/tmp/lab_3_audit_tree.sock"

cd "$PROJECT_DIR"

cleanup() {
  chmod 755 "$ROOT_FS/no_access" 2>/dev/null || true
  rm -rf "$ROOT_FS" 2>/dev/null || true
  rm -f "$SOCK" 2>/dev/null || true
}

wait_for_socket() {
  local timeout=50
  while [[ $timeout -gt 0 ]]; do
    if [[ -S "$SOCK" ]]; then
      return 0
    fi
    sleep 0.1
    timeout=$((timeout - 1))
  done
  return 1
}

trap cleanup EXIT

# 0) prepare test data
./tests/setup_test_fs.sh "$ROOT_FS"

# 1) ensure stale socket removed
rm -f "$SOCK"

# 2) start server in background, redirect output to file
echo "[+] Starting server..."
./server > "$OUT_LOG" 2>&1 &
SERVER_PID=$!
if ! wait_for_socket; then
  echo "[ERROR] Server socket not available: $SOCK"
  exit 1
fi

# 3) run client against test FS
echo "[+] Running client..."
./client --root "$ROOT_FS" --max-depth 10 --max-nodes 5000

# 4) give server a moment to flush
sleep 0.1

# 5) stop server (if still alive)
if kill -0 "$SERVER_PID" 2>/dev/null; then
  kill "$SERVER_PID" 2>/dev/null || true
fi
wait "$SERVER_PID" 2>/dev/null || true

echo "[+] Validating output..."
grep -q "no_access.*{!}" "$OUT_LOG"
grep -q "world_writable.txt.*{\\*}" "$OUT_LOG"
grep -q "link_to_ok@.*{S}" "$OUT_LOG"
grep -q "deep.txt" "$OUT_LOG"

echo "[OK] Done."
echo "Server output saved to: $OUT_LOG"
echo "Open it with: less \"$OUT_LOG\""
