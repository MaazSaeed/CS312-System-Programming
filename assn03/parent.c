#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char** argv) {
	int children = atoi(argv[1]);
	char* file_name = argv[2];
	int char_to_read = atoi(argv[3]);
	
	int file = open(file_name, O_RDONLY);
	if(file == -1) {
		fprintf(stderr, "open() failed");
		exit(EXIT_FAILURE);
	}
	
	char buffer[char_to_read + 1]; 
	memset(buffer, '\0', sizeof(buffer));

	// two pipes for bi-directional communication between parent process and each child process.
	// file pipe 1 allows the parent to write only and the child to read only.
	// file pipe 2 allows the parent to read only and the child to write only.

	int pipes[children][2][2];
	for(int i = 0; i < children; i++) {
		if(pipe(pipes[i][0]) != 0 || pipe(pipes[i][1]) != 0) {
			fprintf(stderr, "pipe() failed");
			exit(EXIT_FAILURE);
		}
	}
	
	int bytes_read;
	
	// The parent process writes the data onto the first pipe with each child.
	int k = 0;
	while((bytes_read = read(file, buffer, sizeof(buffer))) > 0) {
		write(pipes[k][0][1], buffer, sizeof(buffer));
		k = (k + 1) % children;
	}
	
	// the parent should not be able to write to the second pipe.
	for(int i = 0; i < children; i++) {
		close(pipes[i][0][1]);
	}

	// spawning N processes
	for(int i = 0; i < children; i++) {
		pid_t child = fork();
		
		if(child == (pid_t) -1) {
			fprintf(stderr, "fork failed");
			exit(EXIT_FAILURE);
		}
		
		if(child == 0) {
			// inside child process
			close(0);
			dup(pipes[i][0][0]);
			close(pipes[i][0][0]);
			close(pipes[i][0][1]);
			
			char* fp2_w;
			asprintf(&fp2_w, "%d", pipes[i][1][1]); // similar to sprintf however, also allocates the required memory.
			
			execlp("./child", "child", file_name, fp2_w, NULL);
			
			fprintf(stderr, "exec failed");
			exit(EXIT_FAILURE); 
		}
	}
	
	// the parent process shall wait for each child process completion and close any open pipes.
	for(int i = 0; i < children; i++) {
		int bytes_processed = 0;

		printf("Parent process waiting...\n");
		int status;
		pid_t terminated_child_pid = waitpid(-1, &status, 0);

		if (terminated_child_pid == -1) {
		    fprintf(stderr, "waitpid() failed");
		    exit(EXIT_FAILURE);
		}
		
		char buff[BUFSIZ + 1] = {0};
		memset(buff, '\0', sizeof(buff));
		
		int byts = 0;
		byts += read(pipes[i][1][0], buff, sizeof(buff));
		
		close(pipes[i][1][0]);
		printf("Child process with ID: %d terminated, wrote: %s characters!\n", terminated_child_pid, buff);
	}

	exit(EXIT_SUCCESS);
}
