//
// Created by aaron on 9/15/2024.
//

#include "hashmap.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define DEFAULT_HASHMAP_CAPACITY 33
// We want to achieve a balance between bucket amounts and bucket depth ideally
#define TABLE_MAX_LOAD 0.75

#define FNV_OFFSET_BIAS 2166136261u
#define FNV_PRIME 16777619

static uint32_t hashKey(const char* key, int len) {
    uint32_t hash = FNV_OFFSET_BIAS;
    for (int i = 0; i < len; i++) {
        hash ^= FNV_PRIME;
        hash ^= key[i];
    }

    return hash;
}

// Resize the underlying array structure to a size of 'capacity'
static void adjustMapCapacity(Hashmap* map, int capacity) {
    map->capacity = capacity;
    Entry* newEntries = calloc(capacity, sizeof(Entry));
    if (newEntries == NULL) exit(1);
    // Put all the old entries back into the new array
    for (int i = 0; i < map->capacity; i++) {
        Entry* entry = &map->arr[i];
        // iterate through all
        while (entry != NULL && entry->key != NULL) {
            // Currently need to recompute hash of every element every time we reassign.
            // TODO: Cache the computed hash with each entry?
            uint32_t hash = hashKey(entry->key, strlen(entry->key));
            int index = hash % map->capacity;
            Entry* dest = &newEntries[index];
            while (dest->key != NULL) {
                dest = dest->next;
            }
            dest->key = entry->key;
            dest->value = entry->value;

            entry = entry->next;
        }
    }

    free(map->arr);
    map->arr = newEntries;
}

void initHashmap(Hashmap* map) {
    map->size = 0;
    map->arr = calloc(DEFAULT_HASHMAP_CAPACITY, sizeof(Entry));
    map->capacity = DEFAULT_HASHMAP_CAPACITY;
    if (map->arr == NULL) exit(2);
}

static void freeEntries(Entry* entry) {
    if (entry == NULL) return;
    if (entry->next == NULL) {
        free(entry);
        return;
    }
    freeEntries(entry->next);
    free(entry);
}


void deleteHashmap(Hashmap* map) {
    for (int i = 0; i < map->capacity; i++) {
        freeEntries(map->arr[i].next);
    }
    free(map->arr);
}

// Returns the value stored at key. Returns -1 if key is not present
int getValue(Hashmap* map, const char* key) {
    uint32_t hash = hashKey(key, strlen(key));
    int index = hash % map->capacity;
    Entry* entry = &map->arr[index];
    while (entry->key != NULL && strcmp(entry->key, key) != 0) {
        entry = entry->next;
    }
    if (entry->key == NULL) {
        return -1;
    }
    return entry->value;
}

void setValue(Hashmap* map, const char* key, int value) {
    if (map->size + 1 > (map->capacity * TABLE_MAX_LOAD)) {
        adjustMapCapacity(map, map->capacity * 2);
    }
    uint32_t hash = hashKey(key, strlen(key));
    int index = hash % map->capacity;
    Entry* entry = &map->arr[index];
    // If the bucket is completely empty (entry->key == NULL) we plop it down into the bucket
    // If the key is already in the bucket, terminate and just replace with the bew value
    // If neither of the above are true, extend the linked list/bucket with the new entry
    while (entry->key != NULL && strcmp(key, entry->key) != 0 && entry->next != NULL) {
        entry = entry->next;
    }
    // We iterated through all non null entries and did not find the key, therefore it is a completely new entry;
    if (entry->next == NULL && entry->key != NULL) {
        Entry* newEntry = malloc(sizeof(Entry));
        newEntry->key = key;
        newEntry->value = value;
        newEntry->next = NULL;
        entry->next = newEntry;
        map->size += 1;
        return;
    }
    // Completely new entry with no hash collision
    if (entry->key == NULL) map->size += 1;
    // One of the first two conditions above was fulfilled
    entry->key = key;
    entry->value = value;
}

bool deleteKey(Hashmap* map, const char* key) {
    // Find the "bucket" that this key belongs to
    // In the case where the bucket is empty, terminate early
    // If the bucket is non empty and the key happens to be the first, change the
    // underlying array to point to the "next" of that entry as the top of the bucket
    // otherwise iterate through the bucket
    uint32_t hash = hashKey(key, strlen(key));
    int index = hash % map->capacity;
    Entry* bucket = &map->arr[index];
    // The bucket is empty and therefore the key definitely does not exist
    if (bucket->key == NULL) return false;
    if (strcmp(bucket->key, key) == 0) {
        if (bucket->next == NULL) {
            bucket->key = NULL;
            bucket->value = -1;
            return true;
        }
        map->arr[index] = *bucket->next;
        map->size -= 1;
        return true;
    }
    while (bucket->next != NULL && strcmp(bucket->next->key, key) != 0) {
        bucket = bucket->next;
    }
    if (strcmp(bucket->key, key) == 0) {
        bucket->next = bucket->next->next;
        map->size -= 1;
        return true;
    }
    return false;
}

void dumpHashmap(Hashmap* map) {
    for (int i = 0; i < map->capacity; i++) {
        Entry* bucket = &map->arr[i];
        while (bucket != NULL && bucket->key != NULL) {
            // [key] : <value>
            printf("[%s] : <%d>\n", bucket->key, bucket->value);
            bucket = bucket->next;
        }
    }
}

