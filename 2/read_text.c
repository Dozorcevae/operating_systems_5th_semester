#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]){
	if (argc != 2){
		fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
		return 2;
	}

	int fd = open(argv[1], O_RDONLY);
	if (fd < 0){
		fprintf(stderr, "open('%s') failed: %s\n", argv[1], strerror(errno));
		return 1;
	}

	char buf[4096];
	ssize_t n;

	while((n = read(fd, buf, sizeof(buf))) > 0){
		ssize_t off = 0;
		while (off < n){
			ssize_t w = write(STDOUT_FILENO, buf + off, (ssize_t)(n - off));
			if (w < 0){
				fprintf(stderr, "write(stdout) failed: %s\n", strerror(errno));
				close(fd);
				return 1;
			}
			off += w;
		}
	}

	if (n < 0){
		fprintf(stderr, "read('%s') failed: %s\n", argv[1], strerror(errno));
		close(fd);
		return 1;
	}
	
	close(fd);
	return 0;
}	

