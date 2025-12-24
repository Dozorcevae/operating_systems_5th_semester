#include "client_args.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage(const char *argv0){
	fprintf(stderr,
			"Usage: %s [--root PATH] [--max-depth N] [--max-nodes N] [--sock PATH]\n"
			"Defaults:\n"
			" --root	$HOME\n"
			" --max-depth	8\n"
			" --max-nodes	8000\n"
			" --sock	/tmp/lab_3_audit_tree.sock\n",
			argv0);
}

int parse_args(int argc, char **argv, ClientConfig *cfg) {
	char *end = NULL;
	for (int i = 1; i < argc; i++){
		if (strcmp(argv[i], "--root") == 0 && (i + 1) < argc){
			cfg -> root = argv[++i];
		}else if (strcmp(argv[i], "--max-depth") == 0 && (i + 1) < argc){
			long value = strtol(argv[++i], &end, 10);
			if (!end || *end != '\0' || value <= 0) {
				print_usage(argv[0]);
				return 0;
			}
			cfg -> max_depth = (int)value;
		}else if (strcmp(argv[i], "--max-nodes") == 0 && (i + 1) < argc){
			long value = strtol(argv[++i], &end, 10);
			if (!end || *end != '\0' || value <= 0) {
				print_usage(argv[0]);
				return 0;
			}
			cfg -> max_nodes = (int)value;
		}else if (strcmp(argv[i], "--sock") == 0 && (i + 1) < argc){
			cfg -> sock_path = argv[++i];
		}else{
			print_usage(argv[0]);
			return 0;
		}
	}
	return 1;
}
