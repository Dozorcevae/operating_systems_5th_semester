#define _GNU_SOURCE
#include "ipc_client.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

int ipc_write_all(int fd, const char *data, size_t len){
	const char *p = (const char *)data;
	size_t off = 0;
	while (off < len){
		ssize_t n = write(fd, p + off, len - off);
		if (n < 0){
			if(errno == EINTR) continue;
			return 0;
		}
		off += (size_t)n;
	}
	return 1;
}
int ipc_connect_unix(const char *sock_path){
	int fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd < 0) return -1;

	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, sock_path, sizeof(addr.sun_path) - 1);

	// retry for server start, strahovochka
	for (int t = 0; t < 10; t++){
		if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == 0){
			return fd;
		}
		usleep(200000);
	}
	close(fd);
	return -1;
}

