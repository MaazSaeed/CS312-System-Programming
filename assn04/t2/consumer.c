#include <unistd.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <fcntl.h> 

#include <semaphore.h> 

#define TEXT_SZ 2048 
struct shared_use_st { 
char some_text[TEXT_SZ]; 
}; 
#define SEMAPHORE_NAME_EMPTIES "ourSemaphore1.dat" 
#define SEMAPHORE_NAME_FULLS   "ourSemaphore2.dat" 

int main() 
{ 
	int running = 1; 
	void *shared_memory = (void *)0; 
	struct shared_use_st *shared_stuff; 
	int shmid; 
	sem_t *semIDE; 
	sem_t *semIDF; 
	semIDE = sem_open(SEMAPHORE_NAME_EMPTIES, O_CREAT, 0666, 1); 
	semIDF = sem_open(SEMAPHORE_NAME_FULLS, O_CREAT, 0666, 0); 
	if (semIDE == SEM_FAILED) { 
		perror("Can't open semaphore"); 
		exit(EXIT_FAILURE); 
	} 
	if (semIDF == SEM_FAILED) { 
		perror("Can't open semaphore"); 
		exit(EXIT_FAILURE); 
	} 
	srand((unsigned int)getpid());     
	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT); 
	if (shmid == -1) { 
		fprintf(stderr, "shmget failed\n"); 
		exit(EXIT_FAILURE); 
	} 
	/* We now make the shared memory accessible to the program. */ 
	shared_memory = shmat(shmid, (void *)0, 0); 
	if (shared_memory == (void *)-1) { 
		fprintf(stderr, "shmat failed\n"); 
		exit(EXIT_FAILURE); 
	} 
	printf("Memory attached at %p\n", shared_memory); 
	shared_stuff = (struct shared_use_st *)shared_memory; 
	while(running) { 
		sem_wait(semIDF); 
		printf("You wrote: %s", shared_stuff->some_text); 
		if (strncmp(shared_stuff->some_text, "end", 3) == 0) { 
			running = 0; 
		} 
		sem_post(semIDE); 
	} 
	
	if (shmdt(shared_memory) == -1) { 
		fprintf(stderr, "shmdt failed\n"); 
		exit(EXIT_FAILURE); 
	} 
	
	if (shmctl(shmid, IPC_RMID, 0) == -1) { 
		fprintf(stderr, "shmctl(IPC_RMID) failed\n"); 
		exit(EXIT_FAILURE); 
	}
	 
	if(sem_close(semIDE) != 0){ 
		perror("Can't close semaphore"); 
		exit(EXIT_FAILURE); 
	}
	 
	if(sem_close(semIDF) != 0){ 
		perror("Can't close semaphore"); 
		exit(EXIT_FAILURE); 
	}
	 
	if(sem_unlink(SEMAPHORE_NAME_EMPTIES) != 0){ 
		perror("Can't delete semaphore"); 
		exit(EXIT_FAILURE); 
	} 
	
	if(sem_unlink(SEMAPHORE_NAME_FULLS) != 0){ 
		perror("Can't delete semaphore"); 
		exit(EXIT_FAILURE); 
	} 
	
	exit(EXIT_SUCCESS); 
}
