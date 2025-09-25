#include <stdio.h>
#include <stdlib.h>
#include <string.h> 	// For strlen()
#include <ctype.h>  	// For isspace()
#include <stddef.h> 	// For ptrdiff_t
#include <curl/curl.h>	// For curl_url tools (URI validation)

#include "lexer.h" 	// Header file for the lexer components and token/token_list data types
#include "hashmap.h"	// Header file for the hashmap implementation in the tokenization

#define BUF_SIZE 1024 	// maximum buffer size of 1024 bytes

TokenList* create_token_list() {
	TokenList* tl = (TokenList*)malloc(sizeof(TokenList));
	tl->head = NULL;
	tl->tail = NULL;
	tl->size = 0;
	return tl;
}

/*
 * @brief Tokenization entry point
 */

TokenList* build_token_list(char* source_string, char* end_of_buffer) {
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

	LexerState lexer_state = STATE_REQUEST_LINE;
	TokenType token_type = TOKEN_INITIAL;
	Token* current_token = NULL;
	ptrdiff_t keyword_size = 0;
	
	/*
	 * Tokenization entry point
	 * Note: can be made cleaner by creating a helper function for next-word extraction
	 */
	while ( *it != '\0' ) {
		switch (lexer_state) {
			case STATE_REQUEST_LINE:
				/*
				 * Stage 1: find the method (first token)
				 */
				while ( isspace(*it) ) { // skip spaces and colons
					++it;
				}
				start = it; //start of the first keyword
				while ( !isspace(*it) ) { // set points to mark endpoints of first keyword
					++it;	
				}

				keyword_size = it - start;
				snprintf(buf, sizeof(buf), "%.*s", keyword_size, start);
				token_type = tokenize_string(buf, lut);	
				current_token = create_token(buf, token_type);				
				add_token_to_list(tl, current_token);

				/*
				 * URI extraction 
				 */
				while ( isspace(*it) ) { 
					++it;
				}
				start = it;
				while ( !isspace(*it) ) {
					++it;	
				}
				keyword_size = it - start;
				snprintf(buf, sizeof(buf), "%.*s", keyword_size, start);
				token_type = TOKEN_URI;
				current_token = create_token(buf, token_type); // assume URI after method
									       // unless proven otherwise by parser
				add_token_to_list(tl, current_token);

				/*
				 * HTTP protocol version extraction
				 */
				while ( isspace(*it) ) { 
					++it;
				}
				start = it;
				while ( !isspace(*it) ) {
					++it;	
				}
				keyword_size = it - start;
				snprintf(buf, sizeof(buf), "%.*s", keyword_size, start);
				token_type = tokenize_string(buf, lut);
				current_token = create_token(buf, token_type); // assume URI after method
				add_token_to_list(tl, current_token);

				/*
				 * CRLF extraction
				 */
				if ( *it == '\r' && *(it+1) == '\n' ) {
					token_type = TOKEN_CRLF;
					current_token = create_token("\r\n", token_type); 
					add_token_to_list(tl, current_token);
					it += 2;
				} else {
					lexer_state = STATE_INVALID;
				}
				lexer_state = STATE_HEADERS; 
				break;
			case STATE_HEADERS:
				++it;// temp
				break;
			case STATE_BODY:
				break;
			case STATE_END_OF_HEADERS:
				break;
			case STATE_INVALID:
				break;
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
