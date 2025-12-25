#!/usr/bin/env bash

while true; do
  echo
  echo "Удалить файл в корзину (bash)"
  echo "Восстановить файл из корзины (bash)"
  echo "Выполнить бекап (bash)"
  echo "Выполнить восстановление бэкапа (bash)"
  echo "Выход"
  echo
  echo "1) remftrash"
  echo "2) unftrash"
  echo "3) makebackup"
  echo "4) restorebackup"
  echo "5) Exit"
  printf "Выберите действие: "

  read -r CHOICE

  case "$CHOICE" in
    1)
      printf "Введите имя файла (в текущем каталоге): "
      read -r FN
      ./remftrash "$FN"
      ;;
    2)
      printf "Введите имя файла (только имя, без пути): "
      read -r FN
      ./unftrash "$FN"
      ;;
    3)
      ./makebackup
      ;;
    4)
      ./restorebackup
      ;;
    5)
      echo "Bye!"
      exit 0
      ;;
    *)
      echo "Неверный выбор."
      ;;
  esac
done
