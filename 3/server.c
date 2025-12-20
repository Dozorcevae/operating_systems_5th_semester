#define _GNU_SOURCE
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *SOCK_PATH = "/tmp/lab_3_audit_tree.sock";
static int server_fd = -1;

static void cleanup(void) {
    if (server_fd != -1) close(server_fd);
    unlink(SOCK_PATH);
}

static void on_sig(int sig) {
    (void)sig;
    cleanup();
    _exit(0);
}

static void die(const char *msg) {
    perror(msg);
    exit(1);
}

static int parse_line(
    const char *line,
    int *depth,
    int *is_last,
    char *type,
    unsigned *mode_oct,
    char rwx[8],
    char flags[32],
    char name[1024]
) {
    // copy because strtok_r modifies the buffer
    char *tmp = strdup(line);
    if (!tmp) return 0;

    char *save = NULL;
    char *f1 = strtok_r(tmp, "\t", &save);
    char *f2 = strtok_r(NULL, "\t", &save);
    char *f3 = strtok_r(NULL, "\t", &save);
    char *f4 = strtok_r(NULL, "\t", &save);
    char *f5 = strtok_r(NULL, "\t", &save);
    char *f6 = strtok_r(NULL, "\t", &save);
    char *f7 = strtok_r(NULL, "\t", &save);

    if (!f1 || !f2 || !f3 || !f4 || !f5 || !f6 || !f7) {
        free(tmp);
        return 0; // parse failed
    }

    *depth = atoi(f1);
    *is_last = atoi(f2);
    *type = f3[0];
    *mode_oct = (unsigned)strtoul(f4, NULL, 8);

    snprintf(rwx, 8, "%s", f5);
    snprintf(flags, 32, "%s", f6);
    snprintf(name, 1024, "%s", f7);

    free(tmp);
    return 1;
}

static void print_prefix(int depth, const int *last_stack) {
    for (int i = 0; i < depth; i++) {
        fputs(last_stack[i] ? "    " : "|   ", stdout);
    }
}

int main(void) {
    signal(SIGINT, on_sig);
    signal(SIGTERM, on_sig);

    unlink(SOCK_PATH);

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) die("socket");

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) die("bind");
    if (listen(server_fd, 1) < 0) die("listen");

    fprintf(stderr, "Server listening: %s\n", SOCK_PATH);

    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) die("accept");

    int stack_cap = 64;
    int *last_stack = malloc((size_t)stack_cap * sizeof(int));
    if (!last_stack) die("malloc");
    for (int i = 0; i < stack_cap; i++) last_stack[i] = 1;

    char buf[4096];
    char *carry = NULL;
    size_t carry_len = 0;

    while (1) {
        ssize_t n = read(client_fd, buf, sizeof(buf));
        if (n == 0) break;
        if (n < 0) {
            if (errno == EINTR) continue;
            die("read");
        }

        // Join carry + new chunk
        size_t total = carry_len + (size_t)n;
        char *chunk = malloc(total + 1);
        if (!chunk) die("malloc");
        if (carry) memcpy(chunk, carry, carry_len);
        memcpy(chunk + carry_len, buf, (size_t)n);
        chunk[total] = '\0';

        free(carry);
        carry = NULL;
        carry_len = 0;

        // Process lines
        char *p = chunk;
        while (1) {
            char *nl = strchr(p, '\n');
            if (!nl) {
                // Save leftover partial line
                carry_len = strlen(p);
                carry = malloc(carry_len + 1);
                if (!carry) die("malloc");
                memcpy(carry, p, carry_len + 1);
                break;
            }

            *nl = '\0';

            if (strcmp(p, "END") == 0) {
                free(chunk);
                close(client_fd);
                free(carry);
                free(last_stack);
                cleanup();
                return 0;
            }

            int depth = 0, is_last = 0;
            char type = 'O';
            unsigned mode_oct = 0;
            char rwx[8] = "---";
            char flags[32] = "";
            char name[1024] = "";

            if (parse_line(p, &depth, &is_last, &type, &mode_oct, rwx, flags, name)) {
                if (depth >= stack_cap) {
                    int new_cap = depth + 64;
                    int *tmp = realloc(last_stack, (size_t)new_cap * sizeof(int));
                    if (!tmp) die("realloc");
                    for (int i = stack_cap; i < new_cap; i++) tmp[i] = 1;
                    last_stack = tmp;
                    stack_cap = new_cap;
                }

                print_prefix(depth, last_stack);

                // Print node
                if (depth == 0) {
                    printf("%s", name);
                } else {
                    printf("%s %s", is_last ? "└──" : "├──", name);
                }

                // Type marker
                if (type == 'D') fputs("/", stdout);
                else if (type == 'L') fputs("@", stdout);

                // Audit fields
                printf(" [%s] (%04o)", rwx, mode_oct);
                if (flags[0] != '\0') printf(" {%s}", flags);
                fputc('\n', stdout);

                // affects child prefix
                last_stack[depth] = is_last ? 1 : 0;
            }

            p = nl + 1;
        }

        free(chunk);
    }

    close(client_fd);
    free(carry);
    free(last_stack);
    cleanup();
    return 0;
}
