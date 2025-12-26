from graphviz import Digraph
from pathlib import Path


def _base(name: str) -> Digraph:
    d = Digraph(name)
    d.attr(rankdir="TB", fontsize="12")
    d.attr("node", fontname="Arial")
    d.attr("edge", fontname="Arial")
    return d


def remftrash_flow() -> Digraph:
    d = _base("remftrash_flow")

    d.attr("node", shape="oval")
    d.node("s", "Старт")
    d.node("e", "Завершение")

    d.attr("node", shape="parallelogram")
    d.node("in", "Ввод: имя файла\n(1 аргумент)")
    d.node("out_ok", "Сообщение: OK\nID, перенос в корзину")
    d.node("out_err", "Сообщение: ошибка\nи выход")

    d.attr("node", shape="diamond")
    d.node("argc", "Аргументов = 1?")
    d.node("exists", "Файл существует\nв текущем каталоге?")
    d.node("trash_ok", "~/.trash существует?")

    d.attr("node", shape="box")
    d.node("mktrash", "Создать ~/.trash")
    d.node("nextid", "Найти следующий ID\n(1,2,3...)")
    d.node("move", "Переместить файл\nв ~/.trash/<ID>")
    d.node("log", "Записать в ~/.trash.log:\n<ID> - <полный_путь>")

    d.edge("s", "in")
    d.edge("in", "argc")
    d.edge("argc", "exists", label="да")
    d.edge("argc", "out_err", label="нет")
    d.edge("exists", "trash_ok", label="да")
    d.edge("exists", "out_err", label="нет")
    d.edge("trash_ok", "nextid", label="да")
    d.edge("trash_ok", "mktrash", label="нет")
    d.edge("mktrash", "nextid")
    d.edge("nextid", "move")
    d.edge("move", "log")
    d.edge("log", "out_ok")
    d.edge("out_ok", "e")
    d.edge("out_err", "e")

    return d


def unftrash_flow() -> Digraph:
    d = _base("unftrash_flow")

    d.attr("node", shape="oval")
    d.node("s", "Старт")
    d.node("e", "Завершение")

    d.attr("node", shape="parallelogram")
    d.node("in", "Ввод: имя файла\n(только имя, без пути)")
    d.node("out_none", "Сообщение: записей не найдено")
    d.node("out_done", "Сообщение: восстановление выполнено")
    d.node("out_err", "Сообщение: ошибка\nи выход")

    d.attr("node", shape="diamond")
    d.node("argc", "Аргументов = 1?")
    d.node("have", "Есть ~/.trash и ~/.trash.log?")
    d.node("found", "В логе найдены записи\nпо имени файла?")
    d.node("confirm", "Подтвердить восстановление?\n(y/n)")
    d.node("dir_ok", "Каталог исходного пути\nсуществует?")
    d.node("collision", "Файл уже существует\nв целевой папке?")

    d.attr("node", shape="box")
    d.node("scan", "Просканировать ~/.trash.log\n(найти все совпадения)")
    d.node("show", "Показать: ID и original_path")
    d.node("set_home", "Назначить цель: $HOME")
    d.node("set_orig", "Назначить цель: исходная папка")
    d.node("rename", "Выбрать имя:\n<name>.restored_<ID>")
    d.node("restore", "Переместить ~/.trash/<ID>\nв целевую папку")
    d.node("clean", "Удалить запись из ~/.trash.log\nи удалить объект из ~/.trash")

    d.edge("s", "in")
    d.edge("in", "argc")
    d.edge("argc", "have", label="да")
    d.edge("argc", "out_err", label="нет")
    d.edge("have", "scan", label="да")
    d.edge("have", "out_err", label="нет")
    d.edge("scan", "found")
    d.edge("found", "out_none", label="нет")
    d.edge("found", "show", label="да")

    # Для каждой найденной записи — цикл:
    d.edge("show", "confirm")
    d.edge("confirm", "dir_ok", label="да")
    d.edge("confirm", "show", label="нет\n(к следующей записи)")

    d.edge("dir_ok", "set_orig", label="да")
    d.edge("dir_ok", "set_home", label="нет")

    d.edge("set_orig", "collision")
    d.edge("set_home", "collision")

    d.edge("collision", "rename", label="да")
    d.edge("collision", "restore", label="нет")
    d.edge("rename", "restore")
    d.edge("restore", "clean")
    d.edge("clean", "show", label="к следующей записи")

    d.edge("out_none", "e")
    d.edge("out_err", "e")
    d.edge("show", "out_done", label="если записей больше нет")
    d.edge("out_done", "e")

    return d


def makebackup_flow() -> Digraph:
    d = _base("makebackup_flow")

    d.attr("node", shape="oval")
    d.node("s", "Старт")
    d.node("e", "Завершение")

    d.attr("node", shape="parallelogram")
    d.node("in", "Источник: ~/listtask")
    d.node("out_ok", "Сообщение: backup создан/обновлён\n+ обновлён ~/backup-report")
    d.node("out_err", "Сообщение: ошибка\n(нет ~/listtask)")

    d.attr("node", shape="diamond")
    d.node("exists", "~/listtask существует?")
    d.node("active", "Есть Backup-... за\nпоследние 7 дней?")
    d.node("in_backup", "Файл уже есть\nв бэкапе?")
    d.node("same", "Размеры совпадают?")
    d.node("nextfile", "Есть ещё файлы\nв ~/listtask?")

    d.attr("node", shape="box")
    d.node("date", "Определить текущую дату\nYYYY-MM-DD")
    d.node("mknew", "Создать Backup-YYYY-MM-DD")
    d.node("useact", "Выбрать действующий Backup-...")
    d.node("copy", "Скопировать файл\nв каталог бэкапа")
    d.node("ver", "Переименовать старую версию:\n<name>.YYYY-MM-DD")
    d.node("copynew", "Скопировать новую версию\nпод исходным именем")
    d.node("report", "Записать действия\nв ~/backup-report")

    d.edge("s", "date")
    d.edge("date", "exists")
    d.edge("exists", "active", label="да")
    d.edge("exists", "out_err", label="нет")

    d.edge("active", "mknew", label="нет")
    d.edge("active", "useact", label="да")
    d.edge("mknew", "useact")

    # цикл по файлам
    d.edge("useact", "nextfile")
    d.edge("nextfile", "in_backup", label="да")
    d.edge("nextfile", "report", label="нет")
    d.edge("in_backup", "copy", label="нет")
    d.edge("in_backup", "same", label="да")
    d.edge("same", "nextfile", label="да (пропуск)")
    d.edge("same", "ver", label="нет")
    d.edge("ver", "copynew")
    d.edge("copynew", "nextfile")
    d.edge("copy", "nextfile")

    d.edge("report", "out_ok")
    d.edge("out_ok", "e")
    d.edge("out_err", "e")

    return d


def restorebackup_flow() -> Digraph:
    d = _base("restorebackup_flow")

    d.attr("node", shape="oval")
    d.node("s", "Старт")
    d.node("e", "Завершение")

    d.attr("node", shape="parallelogram")
    d.node("out_ok", "Сообщение: файлы восстановлены\nв ~/listtask")
    d.node("out_err", "Сообщение: Backup-* не найден")

    d.attr("node", shape="diamond")
    d.node("have", "Существует Backup-*?")
    d.node("skip", "Файл версионный?\n*.YYYY-MM-DD")
    d.node("more", "Есть ещё файлы\nв Backup-*?")

    d.attr("node", shape="box")
    d.node("find", "Найти самый свежий\nBackup-*")
    d.node("mk", "Создать ~/listtask (если нет)")
    d.node("copy", "Скопировать файл\nв ~/listtask")

    d.edge("s", "find")
    d.edge("find", "have")
    d.edge("have", "mk", label="да")
    d.edge("have", "out_err", label="нет")
    d.edge("mk", "more")
    d.edge("more", "skip", label="да")
    d.edge("more", "out_ok", label="нет")
    d.edge("skip", "more", label="да (пропустить)")
    d.edge("skip", "copy", label="нет")
    d.edge("copy", "more")

    d.edge("out_ok", "e")
    d.edge("out_err", "e")

    return d


def menu_flow() -> Digraph:
    d = _base("menu_flow")

    d.attr("node", shape="oval")
    d.node("s", "Старт")
    d.node("e", "Выход")

    d.attr("node", shape="box")
    d.node("loop", "Бесконечный цикл")
    d.node("show", "Показать меню")
    d.node("read", "Считать выбор")
    d.node("act1", "Пункт 1:\nввести имя файла\nзапустить remftrash")
    d.node("act2", "Пункт 2:\nввести имя файла\nзапустить unftrash")
    d.node("act3", "Пункт 3:\nзапустить makebackup")
    d.node("act4", "Пункт 4:\nзапустить restorebackup")
    d.node("bad", "Неверный выбор:\nсообщение и повтор")

    d.attr("node", shape="diamond")
    d.node("sw", "Выбор = 1/2/3/4/5?")

    d.edge("s", "loop")
    d.edge("loop", "show")
    d.edge("show", "read")
    d.edge("read", "sw")

    d.edge("sw", "act1", label="1")
    d.edge("sw", "act2", label="2")
    d.edge("sw", "act3", label="3")
    d.edge("sw", "act4", label="4")
    d.edge("sw", "e", label="5")
    d.edge("sw", "bad", label="другое")

    d.edge("act1", "loop")
    d.edge("act2", "loop")
    d.edge("act3", "loop")
    d.edge("act4", "loop")
    d.edge("bad", "loop")

    return d


def render_all(out_dir: Path) -> None:
    out_dir.mkdir(parents=True, exist_ok=True)

    diagrams = {
        "remftrash_flow": remftrash_flow(),
        "unftrash_flow": unftrash_flow(),
        "makebackup_flow": makebackup_flow(),
        "restorebackup_flow": restorebackup_flow(),
        "menu_flow": menu_flow(),
    }

    for name, g in diagrams.items():
        # format png, сохраняем в out_dir/<name>.png
        g.format = "png"
        g.render(filename=str(out_dir / name), cleanup=True)


if __name__ == "__main__":
    render_all(Path("diagrams_lab4"))
    print("OK: diagrams saved to ./diagrams_lab4/*.png")
