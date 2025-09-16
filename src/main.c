#include "stdlib.h"
#include "stdio.h"

#include "http_parser.h"
#include "router.h"
#include "server.h"

int main(int argc, char* argv[]) {
	http_parser("Hi");
	server();
	router();
	return 0;
}
