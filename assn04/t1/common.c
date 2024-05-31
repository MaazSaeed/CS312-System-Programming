#include "common.h"

void init_addr(addr_t* address, short int domain) {
	address->sin_family = domain;
	address->sin_addr.s_addr = 0;
	address->sin_port = htons(9999);
	return;
}

bool check_error(int val, char* function_name) {
	if(val == -1) {
		fprintf(stdout, "%s failed\n", function_name);
		exit(EXIT_FAILURE);
	}
	
	return false;
}

void chat(poll_t* fds, int client, char* recepient) {
    int ret = poll(fds, 2, 50000);
    check_error(ret, "poll");
        
    char buffer[SZ + 1] = {0};
    
    if(fds[0].revents & POLLIN) {
        read(0, buffer, SZ);
        send(client, buffer, SZ, 0);
    } 
    else if(fds[1].revents & POLLIN) {
        recv(client, buffer, SZ, 0);
        fprintf(stdout, "%s: %s", recepient, buffer);
    }
}

