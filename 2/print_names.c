#include <stdio.h>

int main(int argc, char *argv[]){
	if(argc != 3){
		fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
		return 2;
	}

	printf("Input file: %s\n", argv[1]);
	printf("Output file: %s\n", argv[2]);
	return 0;
}
