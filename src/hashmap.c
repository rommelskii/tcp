#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "hashmap.h"

#define TABLE_SIZE 10

unsigned long hash_function(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

// Creates and initializes a new hash map
HashMap* hashmap_create() {
    // Allocate memory for the map structure
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    
    // Allocate memory for the array of buckets
    map->buckets = (Entry**)calloc(TABLE_SIZE, sizeof(Entry*));
    
    return map;
}

// Inserts or updates a key-value pair in the hash map
void hashmap_set(HashMap* map, const char* key, int value) {
    // 1. Hash the key to get the bucket index
    unsigned long hash = hash_function(key);
    int index = hash % TABLE_SIZE;

    // 2. Go to the bucket and traverse the linked list
    Entry* current = map->buckets[index];
    while (current != NULL) {
        // If the key already exists, update the value and return
        if (strcmp(current->key, key) == 0) {
            current->value = value;
            return;
        }
        current = current->next;
    }

    // 3. If the key doesn't exist, create a new entry
    Entry* new_entry = (Entry*)malloc(sizeof(Entry));
    new_entry->key = strdup(key); // Use strdup to copy the key
    new_entry->value = value;
    new_entry->next = NULL;

    // Insert the new entry at the head of the linked list
    new_entry->next = map->buckets[index];
    map->buckets[index] = new_entry;
}

// Retrieves a value from the hash map by key
int hashmap_get(HashMap* map, const char* key) {
    unsigned long hash = hash_function(key);
    int index = hash % TABLE_SIZE;

    Entry* current = map->buckets[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value; // Key found
        }
        current = current->next;
    }

    return -1; // Key not found
}

// Frees all memory used by the hash map
void hashmap_destroy(HashMap* map) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry* current = map->buckets[i];
        while (current != NULL) {
            Entry* temp = current;
            current = current->next;
            free(temp->key); // Free the duplicated key
            free(temp);      // Free the entry node
        }
    }
    free(map->buckets); // Free the buckets array
    free(map);          // Free the map structure
}
