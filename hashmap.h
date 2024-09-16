//
// Created by aaron on 9/15/2024.
//

#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdio.h>

typedef struct Entry {
    const char* key;
    int value;
    struct Entry* next;
} Entry;

typedef struct {
    size_t size;
    size_t capacity;
    Entry* arr;
} Hashmap;

void initHashmap(Hashmap* map);
void deleteHashmap(Hashmap* map);
// Returns -1 if 'key' was not present in the hashmap, and the given key's value otherwise
int getValue(Hashmap* map, const char* key);
// Set the value of 'key' to 'value'. If already present in the map, will mutate the
void setValue(Hashmap* map, const char* key, int value);
// Delete the entry with the given key value from our hashmap
bool deleteKey(Hashmap* map, const char* key);
// Return a string representation of the given hashmap
void dumpHashmap(Hashmap* map);

#endif //HASHMAP_H
