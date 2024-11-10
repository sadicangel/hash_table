//
// Created by sadic on 10/11/2024.
//

#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

#include <math.h>

static HT_Entry* HT_NewEntry(const char* key, void* value) {
    HT_Entry* entry = malloc(sizeof(HT_Entry));
    entry->key = strdup(key);
    entry->value = value;
    return entry;
}

static void HT_DeleteEntry(HT_Entry* entry) {
    free(entry->key);
    free(entry->value);
    free(entry);
}

HT_HashTable* HT_NewHashTable(const size_t capacity) {
    HT_HashTable* ht = malloc(sizeof(HT_HashTable));
    ht->capacity = capacity;
    ht->count = 0;
    ht->entries = calloc(capacity, sizeof(HT_Entry*));
    return ht;
}

void HT_DeleteHashTable(HT_HashTable* ht) {
    for(size_t i = 0; i < ht->count; ++i) {
        HT_Entry* entry = ht->entries[i];
        if(entry) {
            HT_DeleteEntry(entry);
        }
    }
    free(ht->entries);
    free(ht);
}

static int HT_HashFunction(const char* key, const int mod, const int prime) {
    size_t hash = 0;
    const size_t len = strlen(key);
    for(size_t i = 0; i < len; ++i) {
        hash += (size_t)pow(prime, (double)(len - (i + 1) * key[i]));
        hash = hash % mod;
    }
    return (int)hash;
}