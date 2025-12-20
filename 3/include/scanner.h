#pragma once

void scan_and_send_tree(
		int sock_fd,
		const char *rood_path,
		int max_depth,
		long max_nodes,
		long *sent_counter
		);
