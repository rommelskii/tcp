#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct Entry {
    char* key;
    int value;
    struct Entry* next;
} Entry;

typedef struct {
    Entry** buckets; // An array of pointers to Entry
} HashMap;

unsigned long 	hash_function(const char* str);

HashMap* 	hashmap_create();

void 		hashmap_set(HashMap* map, const char* key, int value);

int 		hashmap_get(HashMap* map, const char* key);

void 		hashmap_destroy(HashMap* map);

#endif //HASHMAP_H
