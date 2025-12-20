#pragma once
#include <sys/types.h>

char fs_type_char(mode_t mode);
void rwx_for_path(const char *path, char out[4]);
void sanitize_field(char *s);

int cmp_str_ptr(const void *a, const void *b);
int can_open_dir(const char *path); // 1 if opendir ok, else 0
