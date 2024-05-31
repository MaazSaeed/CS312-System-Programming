#ifndef COMMON_H
#define COMMON_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <poll.h>
#include <stdbool.h>

#define SZ 256

typedef struct sockaddr_in addr_t;
typedef struct pollfd      poll_t;

void init_addr(addr_t* address, short int domain);

// doing a favor to the eyes by avoiding a clutter of if-statements for error checks.
bool check_error(int val, char* function_name);

void chat(poll_t* fds, int client, char* recepient);

#endif
