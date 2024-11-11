//
// Created by sadic on 10/11/2024.
//

#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

#include <math.h>

#define HT_PRIME_1 163
#define HT_PRIME_2 157

static HT_Entry *HT_NewEntry(const char *key, const void *value, const size_t size) {
    HT_Entry *entry = malloc(sizeof(HT_Entry));
    entry->key = strdup(key);
    entry->value = malloc(size);
    memcpy((void*)entry->value, value, size);
    return entry;
}

static void HT_DeleteEntry(HT_Entry *entry) {
    free((void*)entry->key);
    free((void*)entry->value);
    free(entry);
}

HT_HashTable *HT_NewHashTable(const size_t capacity) {
    HT_HashTable *ht = malloc(sizeof(HT_HashTable));
    ht->capacity = capacity;
    ht->count = 0;
    ht->entries = calloc(capacity, sizeof(HT_Entry *));
    return ht;
}

void HT_DeleteHashTable(HT_HashTable *ht) {
    for (size_t i = 0; i < ht->count; ++i) {
        HT_Entry *entry = ht->entries[i];
        if (entry) {
            HT_DeleteEntry(entry);
        }
    }
    free(ht->entries);
    free(ht);
}

static int HT_HashFunction(const char *key, const size_t mod, const int prime) {
    size_t hash = 0;
    const size_t len = strlen(key);
    for (size_t i = 0; i < len; ++i) {
        hash += (size_t) pow(prime, (double) (len - (i + 1) * key[i]));
    }
    return (int) (hash % mod);
}

static int HT_GetKeyIndex(const char *key, const size_t buckets, const int attempt) {
    const int hash_a = HT_HashFunction(key, buckets, HT_PRIME_1);
    int hash_b = HT_HashFunction(key, buckets, HT_PRIME_2);
    if (hash_a % buckets == 0) {
        hash_b = 1;
    }
    return (int) ((hash_a + hash_b * attempt) % buckets);
}

static HT_Entry DELETED_ENTRY = {nullptr, nullptr};

static bool HT_IsNullOrDeleted(HT_Entry *entry) {
    return !entry || entry == &DELETED_ENTRY;
}

void HT_Put(HT_HashTable *ht, const char *key, const void *value, const size_t size) {
    HT_Entry *new = HT_NewEntry(key, value, size);
    HT_Entry *old;
    int i = 0;
    int index = 0;
    do {
        index = HT_GetKeyIndex(key, ht->capacity, i);
        old = ht->entries[index];
        if(!HT_IsNullOrDeleted(old) && strcmp(old->key, key) == 0) {
            HT_DeleteEntry(old);
            ht->entries[index] = new;
            return;
        }
        ++i;
    } while (!HT_IsNullOrDeleted(old));
    ht->entries[index] = new;
    ht->count++;
}

const void *HT_Get(HT_HashTable *ht, const char *key) {
    HT_Entry *entry;
    int i = 0;
    do {
        const int index = HT_GetKeyIndex(key, ht->capacity, i);
        entry = ht->entries[index];
        if(!HT_IsNullOrDeleted(entry) && strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        ++i;
    } while (entry);

    return nullptr;
}

bool HT_Remove(HT_HashTable* ht, const char* key) {
    HT_Entry *entry;
    int i = 0;
    do {
        const int index = HT_GetKeyIndex(key, ht->capacity, i);
        entry = ht->entries[index];
        if(!HT_IsNullOrDeleted(entry) && strcmp(entry->key, key) == 0) {
            HT_DeleteEntry(entry);
            ht->entries[index] = &DELETED_ENTRY;
            ht->count--;
            return true;
        }
        ++i;
    } while(entry);

    return false;
}
