#include <stdio.h>
#include <string.h>
#include <time.h>

static void msleep(long ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}

static void trim_newline(char *s) {
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') s[n - 1] = '\0';
}

int main(void) {
    const char *path = "data.txt";
    FILE *f = fopen(path, "r");
    if (!f) {
        perror("fopen(data.txt)");
        return 1;
    }

    // Аналог tail -n 0: начинаем чтение с конца файла
    if (fseek(f, 0, SEEK_END) != 0) {
        perror("fseek");
        fclose(f);
        return 1;
    }

    char buf[512];
    while (1) {
        if (fgets(buf, sizeof(buf), f)) {
            trim_newline(buf);
            puts(buf);

            if (strcmp(buf, "QUIT") == 0) {
                puts("QUIT received. Handler stops.");
                break;
            }
        } else {
            // новых строк нет — ждём и продолжаем
            clearerr(f);
            msleep(200);
        }
    }

    fclose(f);
    return 0;
}
