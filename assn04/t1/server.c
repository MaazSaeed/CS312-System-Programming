#include "common.h"

int main(void) {
   /* Server initialization */
	int server_sockfd;
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	check_error(server_sockfd, "socket");
	
	addr_t server_address;
	init_addr(&server_address, AF_INET);
	
	int bind_ret = bind(server_sockfd, (struct sockaddr*)& server_address, sizeof(server_address));
	check_error(bind_ret, "bind");
	
	int listen_ret = listen(server_sockfd, 1); 
	check_error(listen_ret, "listen");
	/************************/
	
	int client_sockfd;
	client_sockfd = accept(server_sockfd, (void *) 0, (void *) 0);	
	check_error(client_sockfd, "accept");
	
   // two polls allowing the server to both send and receive messages.
   // the first poll takes care of the input via stdin-0
   // the second poll checks to see if there is any data available 
   // to read from socket connected to client.
	poll_t fds[2] = { {0, POLLIN, 0}, {client_sockfd, POLLIN, 0}};
	
	while(1) {
		chat(fds, client_sockfd, "Client");
	}
		
	close(server_sockfd);
	close(client_sockfd);
	
	exit(EXIT_SUCCESS);
}










