from graphviz import Digraph

def build_flowchart(output_path: str = "lab3_flowchart", fmt: str = "png") -> None:
    diagram = Digraph("lab3_flowchart", format=fmt)
    diagram.attr(rankdir="TB", fontsize="12")

    diagram.attr("node", shape="oval")
    diagram.node("start", "Старт")
    diagram.node("end", "Завершение")

    diagram.attr("node", shape="parallelogram")
    diagram.node("input", "Считать аргументы\n(путь, режим, параметры)")
    diagram.node("output", "Вывести результат\nили сообщение об ошибке")

    diagram.attr("node", shape="box")
    diagram.node("init", "Инициализировать ресурсы\n(структуры, логирование)")
    diagram.node("connect", "Установить соединение\nс сервером IPC")
    diagram.node("request", "Сформировать запрос\nи отправить серверу")
    diagram.node("response", "Принять ответ\nи разобрать данные")
    diagram.node("cleanup", "Освободить ресурсы\nи закрыть соединение")

    diagram.attr("node", shape="diamond")
    diagram.node("args_ok", "Аргументы корректны?")
    diagram.node("conn_ok", "Соединение успешно?")
    diagram.node("resp_ok", "Ответ получен?\nДанные валидны?")

    diagram.edge("start", "input")
    diagram.edge("input", "args_ok")
    diagram.edge("args_ok", "init", label="да")
    diagram.edge("args_ok", "output", label="нет")
    diagram.edge("init", "connect")
    diagram.edge("connect", "conn_ok")
    diagram.edge("conn_ok", "request", label="да")
    diagram.edge("conn_ok", "output", label="нет")
    diagram.edge("request", "response")
    diagram.edge("response", "resp_ok")
    diagram.edge("resp_ok", "output", label="да")
    diagram.edge("resp_ok", "output", label="нет")
    diagram.edge("output", "cleanup")
    diagram.edge("cleanup", "end")

    diagram.render(output_path, cleanup=True)


if __name__ == "__main__":
    build_flowchart()
