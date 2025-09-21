#include <stdio.h>
#include <stdlib.h>
#include <string.h> 	// For strlen()
#include <ctype.h>  	// For isspace()
#include <stddef.h> 	// For ptrdiff_t

#include "lexer.h" 	// Header file for the lexer components and token/token_list data types

#define BUF_SIZE 1024 	// maximum buffer size of 1024 bytes

TokenList* create_token_list() {
	TokenList* tl = (TokenList*)malloc(sizeof(TokenList));
	tl->head = NULL;
	tl->tail = NULL;
	tl->size = 0;
	return tl;
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


void delete_token_list(TokenList* tl) {
	if (tl->size == 0) {
		perror("Deletion error: size is 0\n");
		return;
	}
	Token* current_token = tl->head;
	Token* next_token; 

	while (current_token != NULL) {
		next_token = current_token->next;
		free(current_token->str); //free the token's string content first
		free(current_token);
		current_token = next_token;
	}

	tl->head = NULL;
	tl->tail = NULL;
	tl->size = 0;
	// don't forget to call free(tl) on the invoker function
}
