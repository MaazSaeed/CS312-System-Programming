#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {

	for(int i = 0; i < 3; i++) {
		if(fork() == 0) {
			printf("Inside child process, i: %d\n", i);
			
			char *args[] = {"./task02A", NULL};
			execvp(args[0], args);
			
			// only runs if exec fails.
			printf("exec failed\n");
			return 1;
		}
	}
	
	for(int i = 0; i < 3; i++) {
		// waiting for each process (child) to complete
		//wait(NULL);
		int status;
		int* status_ptr = &status;
		pid_t child_id = waitpid(-1, status_ptr, 0);
		
		printf("\n--> Child process %d has stopped, child id: %ld\n\n", i, (long) child_id);

	}
	
	printf("\n----Inside parent process----\n");
	
	return 0;
}
