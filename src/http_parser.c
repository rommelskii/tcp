#include "stdlib.h"
#include "stdio.h"
#include <ctype.h>
#include <string.h>

#include "http_parser.h"

typedef enum {
	CHARACTER,
	WHITE_SPACE
} TokenType;

typedef struct {
	char* str;
	TokenType t_type;
} Token;


int http_parser(char* buffer, size_t message_size) {
	char* left = buffer;
	char* right = left;
	size_t length = 0;
	
	for (size_t i=0; i<message_size; ++i) {
		while ( isspace(*right) != 0 ) {
			right++;
		}
		length = right - left;
		snprintf(buffer, sizeof(buffer), "%.*s", (int)length, left);
	}
}
