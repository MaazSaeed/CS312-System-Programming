#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
	fork();
	fork(); fprintf(stderr, "A");
	fork(); fprintf(stderr, "B");
	
	return 0;
}
