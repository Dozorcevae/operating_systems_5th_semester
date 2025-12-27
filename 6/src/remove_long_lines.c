#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


static void print_usage(const char *prog) {
    fprintf(stderr, "Использование: %s <input> <n> <output>\n", prog);
}

static FILE *open_file(const char *path, const char *mode) {
    FILE *f = fopen(path, mode);
    if (!f) {
        fprintf(stderr, "Ошибка: не удалось открыть файл '%s'\n", path);
        perror("fopen");
        exit(2);
    }
    return f;
}

/* Парсим число n и проверяем, что оно нормальное */
static long parse_n(const char *s) {
    errno = 0;
    char *end = NULL;
    long n = strtol(s, &end, 10);

    if (errno != 0 || end == s || *end != '\0') {
        fprintf(stderr, "Ошибка: n должно быть целым числом, а не '%s'\n", s);
        exit(1);
    }
    if (n <= 0) {
        fprintf(stderr, "Ошибка: n должно быть > 0, а у вас %ld\n", n);
        exit(1);
    }
    return n;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        print_usage(argv[0]);
        return 1;
    }

    const char *infile = argv[1];
    long n = parse_n(argv[2]);
    const char *outfile = argv[3];

    FILE *in = open_file(infile, "r");
    FILE *out = open_file(outfile, "w");

    char *line = NULL;   // сюда getline будет записывать строку
    size_t cap = 0;      // размер выделенной памяти под line
    ssize_t len;         // длина строки, которую getline прочитала

    while ((len = getline(&line, &cap, in)) != -1) {
        /*
          len включает '\n', если он был.
          Для сравнения длины строки логичнее считать длину без '\n'.
        */
        size_t effective_len = (size_t)len;

        if (effective_len > 0 && line[effective_len - 1] == '\n') {
            effective_len--;
        }

        /*
          Если длина строки <= n — оставляем (пишем в выходной файл).
          Иначе пропускаем, то есть "удаляем".
        */
        if ((long)effective_len <= n) {
            fputs(line, out);
        }
    }

    free(line);
    fclose(in);
    fclose(out);

    printf("Готово: удалены строки длиннее %ld из '%s' -> '%s'\n", n, infile, outfile);
    return 0;
}
