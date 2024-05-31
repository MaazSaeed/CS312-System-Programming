#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> // for access to atoi to convert string to integer

int main(int argc, char** argv) {
	int in, out; // file descriptors for input and output file respectively.

	unsigned int block_size = atoi(argv[3]);
	char *block = (char*) calloc(block_size, sizeof(char)); 
	
	unsigned int bytes_read; // stores the number of bytes read when read() is invoked.
	
	char *in_file = argv[1];
	char *out_file = argv[2];
	
	in = open(in_file, O_RDONLY);
	out = open(out_file, O_WRONLY);

	// Error handling in case either of the file can not be opened.
	if(in == -1 || out == -1) {
	perror("Error opening file\n");
	}
	
	while((bytes_read = read(in, block, block_size)))
		write(out, block, bytes_read);
		
	printf("File Size: 1200 MB, Block Size: %d\n", block_size);
	
	exit(0);
}
