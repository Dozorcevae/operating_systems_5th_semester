#pragma once

typeof struct{
	int depth;
	int is_last;
	char type;		 //D/F/L/O
	unsigned mode_oct;	 //0..0777
	char rwx[4];		 // "rwx"
	char flags[16]		 // e.g. "!*S"
	char name[256];		 // sinitized
} Record;

int record_send(int sock_fd, const Record *rec);
int record_send_end(int sock_fd);
