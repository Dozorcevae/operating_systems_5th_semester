#pragma once

typedef struct{
	const char *root;	//default: $HOME
	int max_depth;		//default: 8 (root depth = 0)
	long max_nodes;		//default: 8000
	const char *sock_path	//default: "/tmp/lab_3_audit_tree.sock"
}ClientConfig;

int parse_args(int argc, char **argv, ClientConfig *cfg);
void print_usage(const char *argv0);

