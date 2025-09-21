#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For strlen()
#include <ctype.h>  // For isspace()
#include <stddef.h> // For ptrdiff_t

#define BUF_SIZE 1024


/*
 * file: lexer.c
 * description: implementation file for the lexer with definitions
 *              of the tokens 
 */


typedef enum {
	HTTP_METHOD,
	HTTP_ROUTE,
	HTTP_VERSION,
	HTTP_INVALID
} TokenType;

typedef struct Token {
	char* str;
	struct Token* next;
	TokenType type;
} Token;

typedef struct {
	Token* head;
	Token* tail;
	size_t size;
} TokenList;

TokenList* create_token_list() {
	TokenList* tl = (TokenList*)malloc(sizeof(TokenList));
	tl->head = NULL;
	tl->tail = NULL;
	tl->size = 0;
	return tl;
}

Token* create_token(char* s, TokenType t_type) {
	Token* new_token = (Token*)malloc(sizeof(Token));
	new_token->next = NULL;
	new_token->str = s;
	new_token->type = t_type;
	return new_token;
}

Token* add_token_to_list(TokenList* token_list, Token* token) {
	if (token_list->size == 0) {
		token_list->head = token;
		token_list->tail = token_list->head;
	} else {
		Token* last_element = token_list->tail;
		last_element->next = token;
		token_list->tail = last_element->next;
	}
	token_list->size++;
	return token_list->tail;
}

TokenList* build_token_list(char* source_string) {
	char buf[BUF_SIZE];
	memset(buf, 0, BUF_SIZE);

	if  ( sizeof(source_string) == 0 ) {
		perror("Tokenizer error: cannot build list from empty string\n");
		return NULL;
	}
	
	char* it = source_string;
	char* start = NULL;

	while ( *it != '\0' ) {
		while ( isspace(*it) ) {
			++it;
		}
		if ( *it == '\0' ) {
			break;	
		}
		start = it;
		while ( *it != '\0' && !isspace(*it) ) {
			++it;
		}

		ptrdiff_t length = it - start;
		snprintf(buf, BUF_SIZE, "%.*s", (int)length, start);
	}

	return NULL;
}

void test() {
	char* s = "Hello world";
	TokenList* tl = build_token_list(s);
}


int main() {
	test();	
	return 0;
}
