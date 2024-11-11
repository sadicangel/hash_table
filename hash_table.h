//
// Created by sadic on 10/11/2024.

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define HT_INITIAL_BASE_SIZE 53

/**
 * Represents a key-value pair within a hash table.
 */
typedef struct {
    const char* key;
    const void* value;
} HT_Entry;

/**
 * Represents a hash table.
 */
typedef struct {
    size_t count;
    size_t capacity;
    HT_Entry** entries;
} HT_HashTable;

/**
 * Creates a new hash table with the specified initial capacity.
 * @param capacity The initial number of entries that the hash table can contain.
 * @return A new hash table.
 */
HT_HashTable* HT_NewHashTable(const size_t capacity);

/**
 * Deletes the specified hash table.
 * @param ht The hash table that will be deleted.
 */
void HT_DeleteHashTable(HT_HashTable* ht);

void HT_Put(HT_HashTable* ht, const char* key, const void* value, const size_t size);
const void* HT_Get(HT_HashTable* ht, const char* key);
bool HT_Remove(HT_HashTable* ht, const char* key);

#endif //HASH_TABLE_H
