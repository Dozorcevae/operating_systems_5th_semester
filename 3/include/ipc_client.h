#pragma once
#include <stddef.h>

int ipc_connect_unix(const char *sock_path);
int ipc_write_all(int fd, const void *data, size_t len);
