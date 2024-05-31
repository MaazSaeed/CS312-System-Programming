#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	// calling getpid() to ensure the seed is not same for all the spawned
	// processes.
	srand((long) getpid());
	unsigned short sleep_time = rand() % 6;

	sleep(sleep_time);
	printf("Awhhh Nap time is over: %ds\n", sleep_time);
	printf("Process id: %ld, Parent id: %ld\n", (long) getpid(), (long) getppid());
	
	return 0;
}
