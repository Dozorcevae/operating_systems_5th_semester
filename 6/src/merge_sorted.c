#include <stdio.h>
#include <stdlib.h>

/*
  Программа: merge_sorted

  Идея задания:
  Есть два текстовых файла, где числа уже отсортированы по возрастанию.
  Нужно объединить их в один файл, тоже по возрастанию.

  Запуск:
  ./merge_sorted <input1> <input2> <output>

  Пример:
  ./merge_sorted a.txt b.txt merged.txt
*/

static void print_usage(const char *prog) {
    fprintf(stderr, "Использование: %s <input1> <input2> <output>\n", prog);
}

/* Просто удобная функция, чтобы открывать файл и сразу проверять ошибки */
static FILE *open_file(const char *path, const char *mode) {
    FILE *f = fopen(path, mode);
    if (!f) {
        fprintf(stderr, "Ошибка: не удалось открыть файл '%s'\n", path);
        perror("fopen"); // печатает системную причину ошибки
        exit(2);
    }
    return f;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        print_usage(argv[0]);
        return 1;
    }

    const char *file1 = argv[1];
    const char *file2 = argv[2];
    const char *outfile = argv[3];

    FILE *f1 = open_file(file1, "r");
    FILE *f2 = open_file(file2, "r");
    FILE *out = open_file(outfile, "w");

    /*
      Делаем классическое "слияние", как в merge sort:
      читаем по одному числу из каждого файла и каждый раз
      записываем меньшее в выход.
    */

    long long a, b;

    int has_a = (fscanf(f1, "%lld", &a) == 1);
    int has_b = (fscanf(f2, "%lld", &b) == 1);

    while (has_a && has_b) {
        if (a <= b) {
            fprintf(out, "%lld\n", a);
            has_a = (fscanf(f1, "%lld", &a) == 1);
        } else {
            fprintf(out, "%lld\n", b);
            has_b = (fscanf(f2, "%lld", &b) == 1);
        }
    }

    /*
      Если один файл закончился раньше — просто дописываем остаток второго.
      Потому что там всё уже отсортировано.
    */

    while (has_a) {
        fprintf(out, "%lld\n", a);
        has_a = (fscanf(f1, "%lld", &a) == 1);
    }

    while (has_b) {
        fprintf(out, "%lld\n", b);
        has_b = (fscanf(f2, "%lld", &b) == 1);
    }

    fclose(f1);
    fclose(f2);
    fclose(out);

    printf("Готово: '%s' + '%s' -> '%s'\n", file1, file2, outfile);
    return 0;
}
