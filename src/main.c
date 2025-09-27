#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#include "hashmap.h"
#include "lexer.h"

#define PORT 8080
#define BACKLOG 10 //10 maximum in queue
#define BUFFER_SIZE 1024		   

/*
void test_tokenize() {
	HashMap* lut = lut_create();	
	char* src = "GET http://localhost:8080 HTTP/1.1";
	TokenList* tl = build_token_list(src);

	print_token_list(tl);
}

int main(void) {
	test_tokenize();
}
*/

int main() {
	struct sockaddr_in address; 
	int server_fd;
	int address_length = sizeof(address);

	char buf[BUFFER_SIZE];

	memset(buf, 0, BUFFER_SIZE); //initialize buffer to 0

	server_fd = socket(AF_INET, SOCK_STREAM, 0); 
	if (server_fd == -1) {
		perror("Error opening socket. Exiting...\n");
		exit(EXIT_FAILURE);
	}


	//set socket for debug mode
	int opt = 1;
	if ( setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	
	if ( bind(server_fd, (struct sockaddr*)&address, address_length) < 0 ) {
		perror("Error binding socket. Exiting...\n");
		exit(EXIT_FAILURE);
	}

	printf("Successfully bound socket to port %d\n", PORT);


	// listening state 
	if ( listen(server_fd, BACKLOG) < 0 ) {
		perror("Error listening to socket file descriptor. Exiting...\n");
		exit(EXIT_FAILURE);
	}

	printf("Currently listening at port %d\n", PORT);

	//accept logic
	while (1) {
		int new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&address_length);

		if (new_socket < 0) {
			perror("Error accepting the inbound packet. Exiting...\n");
			continue;
		}

		ssize_t bytes_received = recv(new_socket, buf, BUFFER_SIZE, 0);
		if ( bytes_received > 0 ) {
			buf[bytes_received] = '\0';
			const char* end_of_buffer = buf+bytes_received;
			TokenList* tl = build_token_list(buf, end_of_buffer); // lexer
			print_token_list(tl);
		} else {
			perror("recv failed");
		}
		//printf("Bytes received: %li\nContent: %s\n", bytes_received, buf);

		close(new_socket);

	}

	close(server_fd);
	return 0;
}
