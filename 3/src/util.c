#include "util.h"

#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

char fs_type_char(mode_t mode){
	if (S_ISDIR(mode)) return 'D';
	if (S_ISREG(mode)) return 'F';
	if (S_ISLNK(mode)) return 'L';
	return 'O';
}

void rwx_for_path(const char *path, char out[4]){
	out[0] = (access(path, R_OK) == 0) ? 'r' : '-';
	out[1] = (access(path, W_OK) == 0) ? 'w' : '-';
	out[2] = (access(path, X_OK) == 0) ? 'x' : '-';
	out[3] = '\0';
}

void sanitize_field(char *s){
	for (; *s; *s++){
		if (*s == '\t' || *s == '\n' || *s == '\r') *s = ' ';
	}
}

int cmp_str_ptr(const void *a, const void *b){
	const char *sa = *(const char * const *)a;
	const char *sb = *(const char * const *)b;
	return strcmp(sa, sb);
}

int can_open_dir(const char *path){
	DIR *d = opendir(path);
	if (!d) return 0;
	closedir(d);
	return 1;
}
