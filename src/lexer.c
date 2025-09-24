#include <stdio.h>
#include <stdlib.h>
#include <string.h> 	// For strlen()
#include <ctype.h>  	// For isspace()
#include <stddef.h> 	// For ptrdiff_t
#include <curl/curl.h>	// For curl_url tools (URI validation)

#include "lexer.h" 	// Header file for the lexer components and token/token_list data types
#include "hashmap.h"	// Header file for the hashmap implementation in the tokenization

#define BUF_SIZE 1024 	// maximum buffer size of 1024 bytes
			//

int is_uri(char* str) {
	CURLU *url = curl_url();
	if (!url) {
		return 0;
	}

	CURLUcode rc = curl_url_set(url, CURLUPART_URL, str, 0);
	curl_url_cleanup(url);

	return (rc == CURLUE_OK);
}

TokenList* create_token_list() {
	TokenList* tl = (TokenList*)malloc(sizeof(TokenList));
	tl->head = NULL;
	tl->tail = NULL;
	tl->size = 0;
	return tl;
}

TokenList* build_token_list(char* source_string) {
	TokenList* tl = create_token_list(); // initialize token list
	HashMap* lut = lut_create();
	char buf[BUF_SIZE];
	memset(buf, 0, BUF_SIZE);

	if  ( sizeof(source_string) == 0 ) {
		perror("Tokenizer error: cannot build list from empty string\n");
		return NULL;
	}
	
	char* it = source_string;
	char* start = NULL;

	TokenType token_type;
	TokenType prev_token = 0;
	Token* new_token;

	/*
	 * Tokenization entry point
	 */
	while ( *it != '\0' ) {
		if ( isspace(*it) ) { // check for spaces
			new_token = create_token(" ", TOKEN_SPACE);
			add_token_to_list(tl, new_token);
			++it;
		}
		else if ( *it == ':' ) { // check for colons
			new_token = create_token(":", TOKEN_COLON);
			add_token_to_list(tl, new_token);
			++it;
			continue;
		} else { 		
			// otherwise, begin keyword extraction
			start = it;
			while ( *it != '\0' && !isspace(*it) && *it != ':') {
				++it;
			}
			ptrdiff_t length = it - start;
			snprintf(buf, BUF_SIZE, "%.*s", (int)length, start);

			// begin entry point for tokenization
			token_type = tokenize_string(buf, lut);
			if (token_type == TOKEN_ILLEGAL) {
				switch (prev_token)  {
					case TOKEN_METHOD:
						token_type = TOKEN_URI;
						break;
					case TOKEN_HEADER_KEY:
						token_type = TOKEN_HEADER_VALUE;
						break;
				}
			}
			prev_token = token_type;
			Token* new_token = create_token(buf, token_type);
			add_token_to_list(tl, new_token);
		}
	}

	return tl;
}


Token* create_token(char* s, TokenType t_type) {
	Token* new_token = (Token*)malloc(sizeof(Token));
	new_token->next = NULL;
	new_token->prev = NULL;
	new_token->type = t_type;

	new_token->str = (char*)malloc(strlen(s)+1);
	strcpy(new_token->str, s);

	return new_token;
}

Token* add_token_to_list(TokenList* token_list, Token* token) {
	if (token_list == NULL || token == NULL) {
		return NULL;
	}
	if (token_list->size == 0) {
		token_list->head = token;
		token_list->tail = token;
	} else {
		token_list->tail->next = token;
		token->prev = token_list->tail;
		token_list->tail = token;
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

void print_token_list(TokenList* tl) {
	if (tl->size == 0) {
		printf("Token list is empty.");
	} 

	printf("Size of token list: %zu\n", tl->size);
	
	Token* it = tl->head;
	while (it != NULL) {
		switch(it->type) {
			case TOKEN_METHOD:
				printf("String: %s Type: Method\n", it->str);
				break;
			case TOKEN_URI:
				printf("String: %s Type: URI\n", it->str);
				break;
			case TOKEN_VERSION:
				printf("String: %s Type: Version\n", it->str);
				break;
			case TOKEN_HEADER_KEY:
				printf("String: %s Type: Header Key\n", it->str);
				break;
			case TOKEN_HEADER_VALUE:
				printf("String: %s Type: Header Value\n", it->str);
				break;
			case TOKEN_BODY:
				printf("String: %s Type: Body\n", it->str);
				break;
			case TOKEN_COLON:
				printf("String: %s Type: Colon\n", it->str);
				break;
			case TOKEN_SPACE:
				printf("String: %s Type: Space\n", it->str);
				break;
			case TOKEN_CRLF:
				printf("String: %s Type: CRLF\n", it->str);
				break;
			case TOKEN_END_OF_HEADERS:
				printf("String: %s Type: End of Headers\n", it->str);
				break;
			case TOKEN_EOF:
				printf("String: %s Type: EOF\n", it->str);
				break;
			case TOKEN_ILLEGAL:
				printf("String: %s Type: Illegal\n", it->str);
				break;
		}
		it = it->next;
	}
}

TokenType tokenize_string(char* buf, HashMap* lut) {
	TokenType t_type = hashmap_get(lut, buf);	//LUT access
	return t_type;
}
