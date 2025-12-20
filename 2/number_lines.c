#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

static int write_all(int fd, const char *buf, size_t len){
	size_t off = 0;
	while (off < len){
		ssize_t w = write(fd, buf + off, len - off);
		if (w < 0) return -1;
		off += (size_t)w;
	}
	return 0;
}

int main (int argc, char *argv[]){
	if (argc != 3){
		fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
		return 2;
	}

	const char *in_path = argv[1];
	const char *out_path = argv[2];

	int in_fd = open(in_path, O_RDONLY);
	if (in_fd <  0) {
		fprintf(stderr, "open ('%s') failed: %s\n", in_path, strerror(errno));
		return 1;
	}

	int out_fd = open(out_path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (out_fd <  0) {
		fprintf(stderr, "open ('%s') failed: %s\n", out_path, strerror(errno));
		close(in_fd);
		return 1;
	}

	unsigned long long line_no = 1;
	int at_line_start = 1;

	char buf[4096];
	ssize_t n;

	while((n = read(in_fd, buf, sizeof(buf))) > 0){
		for(ssize_t i = 0; i < n; i++){
			char c = buf[i];

			if(at_line_start){
				char prefix[32];
				int k = snprintf(prefix, sizeof(prefix), "%04llu: ", line_no);
				if (k < 0 || k >= (int)sizeof(prefix)){
					fprintf(stderr, "sprintf failed\n");
					close(in_fd);
					close(out_fd);
					return 1;
				}
				if (write_all(out_fd, prefix, (size_t)k) < 0){
					fprintf(stderr, "write(output) failed %s\n", strerror(errno));
					close(in_fd);
					close(out_fd);
					return 1;
				}
				at_line_start = 0;
			}
			if (write_all(out_fd, &c, 1) < 0){
				fprintf(stderr, "write(output) failed %s\n", strerror(errno));
				close(in_fd);
				close(out_fd);
				return 1;
			}

			if (c == '\n'){
				line_no++;
				at_line_start = 1;
			}
		}
	}

	if (n < 0){
		fprintf(stderr, "read('%s') failed: %s\n", in_path, strerror(errno));
		close(in_fd);
		close(out_fd);
		return 1;
	}

	close(in_fd);
	close(out_fd);
	return 0;
}
