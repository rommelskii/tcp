#ifndef LEXER_H
#define LEXER_H

/*
 * file: lexer.h
 * description: header file for the lexer with definitions
 *              of the tokens, token lists, and the helper functions for creating the said structures.
 */


/**
 * @brief An enumeration of the possible token types of inbound HTTP requests.
 * Still in prototyping stage.
 */

typedef enum {
	TOKEN_METHOD,
	TOKEN_URI,
	TOKEN_VERSION,
	TOKEN_HEADER_KEY,
	TOKEN_HEADER_VALUE,
	TOKEN_BODY,
	TOKEN_COLON,
	TOKEN_SPACE,
	TOKEN_CRLF,
	TOKEN_END_OF_HEADERS,
	TOKEN_EOF,
	TOKEN_ILLEGAL
} TokenType;

/**
 * @brief An abstract data type for a token.
 *
 */

typedef struct Token {
	char* str;
	struct Token* next;
	TokenType type;
} Token;

/**
 * @brief A linked-list definition of a tokenized string. 
 *
 * Each token list contains a head and tail Token pointers, and a size attribute.
 */

typedef struct {
	Token* head;
	Token* tail;
	size_t size;
} TokenList;

/**
 * @brief Creates a token list. 
 * @return A pointer to the newly created token list.
 */
TokenList* 	create_token_list();

/**
 * @brief Creates a token list from a null-terminated source string.
 *
 * This function utilizes the lexer in determining the token type.
 *
 * @param source_string A pointer to a null-terminated character array/string. 
 * @return A pointer to the newly created token list.
 */
TokenList* 	build_token_list(char* source_string);

/**
 * @brief Creates an individual token with a defined string s and token type t_type.
 * @param s The pointer to the character array of the token to be made from.
 * @param t_type The type of token to be assigned to it.
 * @return A pointer to the newly created token. 
 */
Token* 		create_token(char* s, TokenType t_type);

/**
 * @brief Appends a token to the back of the token list.
 * @param token_list The pointer to a built token list.
 * @param token The token to be added.
 * @return A pointer to the newly appended token on the token list.
 */
Token* 		add_token_to_list(TokenList* token_list, Token* token);


/**
 * @brief Deletes a token list by freeing the elements back to the memory.
 * @param tl The pointer to the token list to be deleted.
 */
void 		delete_token_list(TokenList* tl);

/**
 * @brief Given a string, it will output the corresponding token type.
 * @param buf Pointer to the buffer in the main entry point.
 * @return The token type of the current buffer content.
 */
TokenType	tokenize_string(char* buf);

#endif // LEXER_H
