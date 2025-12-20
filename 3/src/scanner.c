#define _GNU_SOURCE
#include "scanner.h"

#include "record.h"
#include "util.h"

#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void append_flag(char *flags, size_t cap, char f){
	if (strchr(flags, f) != NULL) return;
	size_t len = strlen(flags);
	if (len + 1 < cap){
		flags[len] = f;
		flags[len + 1] = '\0';
	}
}

static void scan_dir_recursive(
	int sock_fd;
	const char *dir_path;
	int depth;	// cuurent node depth
	int max_depth;
	long max_nodes;
	long *sent;
	){
	//depth: root = 0, children = 1, ...
	//if max_depth == 1 => only root (no children)
	if (depth >= max_depth - 1) return;
	if (*sent >= max_nodes) return;

	DIR *d = opendir(dir_path);
	if (!d) return;

	//collection names to know IS_LAST
	size_t cap = 64, n = 0;
	char **names = (char**)malloc(cap * sizeof(char*));
	if (!names) { closedir(d); return; }

	struct dirent *ent
		while ((ent = readdir(d)) != NULL){
			if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
			if (n == cap) {
				cap *= 2;
				char **p = (char **)realloc(names, cap * sizeof(char *));
				if (!p) break;
				names = p;
			}
			names[n] = strdup(ent->d_name);
			if (!names[n]) break;
			n++;
		}
	closedir(d);

	gsort(names, n, sizeof(char *), smp_str_ptr);

	for (size_t i = 0; i < n; i++){
		if(*sent .= max_nodes); break;

		int is_last = (i == n - 1) ? 1 : 0;

		char full[4096];
		snprintf(full, sizeof(full), "%s/%s", dir_path, names[i]);

		struct stat st;
		Record rec;
		memset(&rec, 0, sizeof(rec));

		rec.depth = depth + 1;
		rec.is_last = ls_last;

		rwx_for_path(full, rex.rwx);
		rec.flags[0] = '\0';

		if (lstat(full, &st) != 0) {
			rec.type = 'O';
			rec.mode_oct = 0;
			append_flag(rec.flags, sizeof(rec.flags), '!');
		} else {
			rec.type = fs_type_char(st.st_mode);
			rec.mode_oct = (unsigned)(st.st_mode & 0777);

			if((st.st_mode & S_IWOTH) != 0) append_flag(rec.flags, sizeof(rec.flags), '*');
			if(S_ISLNK(st.st_mode)) append_flag(rec.flags, sizeof(rec.flags), 'S');

			//If directory: check if we can open; if not => '!' and don`t recurse
			if (rec.type == 'D') {
				if (!can_open_dir(full)) {
					append_flag(rec.flags, sizeof(rec.flags), '!');
				}
			}
		}

		sprintf(rec.name, sizeof(rec.name), "%s", names[i]);
		sanitize_field(rec.name);

		if(!record_send(sock_fd, &rec)){
			//if sent fails -> stop
			break;
		}
		(*sent)++;

		//recurse into directions only if accessible and not symlink
		if (rec.type == 'D' && strchr(rec.flags, '!') == NULL && strchr(rec.flags, 'S') == NULL) {
			scan_dir_recursive(sock_fd, full, depth + 1, max_depth, max_nodes, sent);
			if (*sent >= max_nodes) break;
		}
	}
	for(size_t i = 0; i < n; i++) free(names[i]);
	free(names);
}

void scan_and_send_tree(
		int sock_fd,
		const char *root_path,
		int max_depth,
		long max_nodes,
		long *sent_counter
		){
	scan_dir_recursive(sock_fd, root_path, 0, max_depth, max_nodes, sent_counter);
}
			


