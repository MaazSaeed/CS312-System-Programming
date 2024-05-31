#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char** argv) {
	char* in_file_name = argv[1];
	      
	pid_t pid = getpid();
	
	char* ptoa; // pid to ascii.
	asprintf(&ptoa, "%d", pid); // similar to sprintf however also handles memory allocation of the resulting string.

	char* out_file_name = NULL;
	asprintf(&out_file_name, "%s-%ld", in_file_name, pid); //filename-pid
	  
	int out_file = open(out_file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if(out_file == -1) {
		fprintf(stderr, "Error creating file\n");
		exit(EXIT_FAILURE);
	}
	
	char buffer[BUFSIZ + 1] = {0};
	memset(buffer, '\0', sizeof(buffer));
	
	int data_processed = 0;
	int char_written = 0;
	while((data_processed = read(0, buffer, sizeof(buffer))) > 0) {
		write(out_file, buffer, sizeof(buffer));
		char_written += data_processed;
	}
	close(out_file);
	
	fprintf(stdout, "Writing to %s file successful!\n", out_file_name);
	int fp2_w = atoi(argv[2]);
	char* bytes_written;
	asprintf(&bytes_written, "%d", char_written);
	write(fp2_w, bytes_written, strlen(bytes_written));

	free(bytes_written);
	free(ptoa);
	free(out_file_name);

	exit(EXIT_SUCCESS);
}
