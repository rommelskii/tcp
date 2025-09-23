#ifndef HASHMAP_H
#define HASHMAP_H

#include "lexer.h"

typedef struct Entry {
    char* key;
    TokenType t_type;
    struct Entry* next;
} Entry;

typedef struct HashMap {
    Entry** buckets; // An array of pointers to Entry
} HashMap;

unsigned long 	hash_function(const char* str);

HashMap* 	hashmap_create();

HashMap*	lut_create(); 

void 		hashmap_set(HashMap* map, const char* key, TokenType t_type);

TokenType 	hashmap_get(HashMap* map, const char* key);

void 		hashmap_destroy(HashMap* map);

#endif //HASHMAP_H
