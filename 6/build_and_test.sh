#!/usr/bin/env bash
set -euo pipefail

# Я сделала скрипт, чтобы не вводить одно и то же много раз.
# Он:
# 1) собирает проект через meson+ninja
# 2) создаёт тестовые файлы
# 3) запускает обе программы и показывает результат

BUILDDIR="builddir"

echo "== Проверяю, что meson и ninja установлены =="
command -v meson >/dev/null 2>&1 || { echo "meson не найден. Установи: sudo dnf install -y meson"; exit 1; }
command -v ninja >/dev/null 2>&1 || { echo "ninja не найден. Установи: sudo dnf install -y ninja-build"; exit 1; }

echo "== Конфигурирую сборку (meson setup) =="
if [ ! -d "$BUILDDIR" ]; then
  meson setup "$BUILDDIR"
fi

echo "== Собираю проект (ninja) =="
ninja -C "$BUILDDIR"

echo
echo "== ТЕСТ 1: merge_sorted (слияние двух отсортированных файлов) =="

cat > a.txt <<'DATA'
1
3
7
10
10
15
DATA

cat > b.txt <<'DATA'
2
3
4
11
14
20
DATA

"$BUILDDIR/merge_sorted" a.txt b.txt merged.txt
echo "-- Результат merged.txt:"
cat merged.txt

echo
echo "== ТЕСТ 2: remove_long_lines (удаление строк длиннее n) =="

cat > lines.txt <<'DATA'
short
1234567890
12345678901
hello world
ok
veryveryverylonglineeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
DATA

# n=10, значит оставятся строки длиной <= 10
"$BUILDDIR/remove_long_lines" lines.txt 10 filtered.txt
echo "-- Результат filtered.txt:"
cat filtered.txt

echo
echo "== Всё готово =="
