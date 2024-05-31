#define TEXT_SZ 2048 

struct shared_use_st { 
	char some_text[TEXT_SZ]; 
};
 
#define SEMAPHORE_NAME_EMPTIES "/ourSemaphore1.dat" 
#define SEMAPHORE_NAME_FULLS   "/ourSemaphore2.dat"
