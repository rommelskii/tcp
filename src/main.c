#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define BACKLOG 10 //10 maximum in queue

int main() {
	struct sockaddr_in address; 
	int server_fd;
	int address_length = sizeof(address);

	server_fd = socket(AF_INET, SOCK_STREAM, 0); 
	if (server_fd == -1) {
		perror("Error opening socket. Exiting...\n");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	
	if ( bind(server_fd, (struct sockaddr*)&address, address_length) < 0 ) {
		perror("Error binding socket. Exiting...\n");
		exit(EXIT_FAILURE);
	}

	printf("Successfully boudn socket to port %d\n", PORT);


	// listening state 
	if ( listen(server_fd, BACKLOG) < 0 ) {
		perror("Error listening to socket file descriptor. Exiting...\n");
		exit(EXIT_FAILURE);
	}

	printf("Currently listening at port %d\n", PORT);

	//accept logic
	/*
	new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&
	*/

	close(server_fd);
	return 0;
}
