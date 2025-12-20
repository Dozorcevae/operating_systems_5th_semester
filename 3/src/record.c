#include "record.h"
#include "ipc_client.h"

#include <stdio.h>
#include <string.h>

int record_send(int sock_fd, const Record *rec){
	//Protocol:
	//DEPTH\tIS-LAST\tTYPE\tMODE_OCT\tRWX\tFLAGS\tNAMES\n
	char line[1024];
	int n = snprintf(line, sizeof(line), 
			"%d\t%d\t%c\t%o\t%s\t%s\n",
			rec->depth,
			rec->is_last,
			rec->type,
			rec->mode_oct,
			rec->rwx,
			rec->flags,
			rec->names);
	if (n <= 0) return 0;
	if ((size_t)n >= sizeof(line)) return 0;

	return ipc_write_all(sock_fd, line, (size_t)n);
}

int record_send_end(int sock_fd){
	return ipc_write_all(sock_fd, 'END\n", 4);
}
