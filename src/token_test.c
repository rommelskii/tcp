#include <stdio.h>
#include <stdlib.h>

#include "hashmap.h"
#include "lexer.h"

void token_test() {
	HashMap* lut = lut_create();	
	const char* key = "POST";
	TokenType t;

	t = hashmap_get(lut, key);

	if (t == TOKEN_METHOD) {
		printf("success\n");
	}
}
