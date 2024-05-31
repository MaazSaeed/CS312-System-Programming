#include "common.h"

int main(void) {
	int client_sockfd;
	client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
   check_error(client_sockfd, "socket");
		
	addr_t client_address;
	init_addr(&client_address, AF_INET);
		
	connect(client_sockfd, (struct sockaddr*) &client_address, sizeof(client_address));

	// same story as in the server.
	poll_t fds[2] = { {0, POLLIN, 0}, {client_sockfd, POLLIN, 0}};
	
	while(1) {
		chat(fds, client_sockfd, "Server");
	}
		
	close(client_sockfd);

	exit(EXIT_SUCCESS);
}
