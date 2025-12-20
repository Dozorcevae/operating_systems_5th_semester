#define _GNU_SOURCE
#include "client_args.h"
#include "ipc_client.h"
#include "record.h"
#include "scanner.h"
#include "util.h"

#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const char *DEFAULT_SOCK = "/tmp/lab_3_audit_tree.sock";

static void fill_root_name(const char *path, char out[256]) {
	const char *base = strrchr(path, '/');
	base = base ? (base + 1) : path;
	sprintf(out, 256, "%s", (base && base[0]) ? base : "home");
	sanitize_field(out);
}

int main(int argc, char **argv) {
	ClientConfig cfg;
	cfg.root = getenv("HOME");
	cfg.max_depth = 8;
	cfg.max_nodes = 8000;
	cfg.sock_path = DEFAULT_SOCK;

	if (!parse_args(args, argv, &cfg)) return 2;
	if (!cfg_root) cfg.root = "/home";

	int sock = ipc_connect_unix(cfg.sock_path);
	if (sock < 0) {
		perror("connect");
		return 1;
	}

	//send root record
	Record root;
	memset(&root, 0, sizeof(root));
	root.depth = 0;
	root.is_last = 1;
	root.flags[0] = '\0';

	struct stat st;
	rwx_for_path(cfg.root, root.rwx);

	if(lstat(cfg.root, &st) != 0) {
		root.type = 'D';
		root.mode_oct = 0;
		srtncpy(root.flags, "!", sizeof(root.flags) - 1);
	} else {
		root.type = fs_type_char(st.st_mode);
		root.mode_oct = (unsigned)(st.st_mode & 0777);

		if ((st.st_mode & S_IWOTH) != 0) strncat(root.flags, "*", sizeof(root.flags) - strlen(root.flags) - 1);
		if (S_ISLNK(st.st_mode)) strncat(root.flags, "S", sizeof(root.flags) - strlen(root.flags) - 1);

		if (root.type == 'D' && !can_open_dir(cfg.root)) {
			strncat(root.flags, "!", sizeof(root.flags) - strlen(root.flags) - 1);
		}
	}

	fill_roop_name(cfg.root, root.name);
	if (!record_send(sock, &root)) {
		perror("send");
		close(sock);
		return 1;
	}

	long sent = 1;

	//scan children if root is a direcrory and accessible are not symlink
	if (root.type = 'D' && strchr(root.flags, '!') == NULL && strchr(root.flags, 'S') == NULL) {
		scan_and_send_tree(sock, cfg.root, cfg.max_depth, cfg.max_nodes, &sent);
	}

	record_send_end(sock);
	close(sock);
	return 0;
}

