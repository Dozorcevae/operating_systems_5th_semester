#include <stdio.h>
#include <string.h>

static void trim_newline(char *s){
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') s[n - 1] = '\0';
}

int main(void){
    const char *path = "data.txt";
    FILE *out = fopen(path, "a");
    if (!out){
        perror("fopen(data.txt)");
        return 1;
    }

    char line[512];
    while (fgets(line, sizeof(line), stdin)){
        trim_newline(line);

        fprintf(out, "%s\n", line);
        fflush(out);

        if(strcmp(line, "QUIT") == 0){
            break;
        }
    }

    fclose(out);
    return 0;
}