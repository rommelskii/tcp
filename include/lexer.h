#ifndef LEXER_H
#define LEXER_H


/**
 * file: lexer.h
 * description: header file for the lexer with definitions
 *              of the tokens, token lists, and the helper functions for creating the said structures.
 */

typedef struct HashMap HashMap;

/**
 * @brief An enumeration of the possible lexer states/stages.
 */
typedef enum {
	STATE_REQUEST_LINE,
	STATE_HEADERS,
	STATE_BODY,
	STATE_INVALID
} LexerState;

/**
 * @brief An enumeration of the possible token types of inbound HTTP requests.
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
	TOKEN_EOF,
	TOKEN_ILLEGAL, 
	TOKEN_INITIAL
} TokenType;

/**
 * @brief An abstract data type for a token.
 */

typedef struct Token {
	char* str;
	struct Token* next;
	struct Token* prev;
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

/*
 * @brief Converts a valid C string to a linked list of tokens.
 * @param source_string Valid C string to be converted.
 * @param end_of_buf  Pointer to the end of the buffer (dictated by the number of bytes received)
 * @return A pointer to the tokenized string.
 */
TokenList* 	build_token_list(char* source_string, const char* end_of_buffer);

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
 * @brief Outputs the content and corresponding token type of a non-empty token list.
 * @param tl The pointer to the token list to be printed.
 */
void 		print_token_list(TokenList* tl);

/**
 * @brief Wrapper function around the hashmap_get for checking the token type of a keyword.
 * @param buf Pointer to the buffer containing the keyword to be tokenized.
 * @param lut Pointer to the HashMap object acting as the LUT created by lut_create()
 */
TokenType	tokenize_string(char* buf, HashMap* lut);

#endif // LEXER_H
