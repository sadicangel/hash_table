//
// Created by sadic on 10/11/2024.
//

#include "hash_table.h"
#include <stdlib.h>
#include <string.h>

#include <math.h>
#include <stdio.h>

/**
 * @def HT_PRIME_1
 * A prime number used as the first hash constant in double hashing.
 */
#define HT_PRIME_1 163

/**
 * @def HT_PRIME_2
 * A prime number used as the second hash constant in double hashing.
 *
 * This prime is distinct from @ref HT_PRIME_1 to ensure the secondary hash
 * function provides a unique offset, helping to resolve collisions.
 */
#define HT_PRIME_2 157

/**
 * @brief A placeholder entry used to mark deleted elements in the hash table.
 */
static HT_Entry DELETED_ENTRY = {nullptr, nullptr};

/**
 * @brief Checks if a hash table entry is either NULL or marked as deleted.
 *
 * @param entry Pointer to the hash table entry to check.
 * @return True if the entry is NULL or marked as deleted, false otherwise.
 */
static bool HT_IsNullOrDeleted(HT_Entry *entry) { return !entry || entry == &DELETED_ENTRY; }

/**
 * @brief Creates a new hash table entry for a given key-value pair.
 *
 * Allocates memory for the entry, duplicates the key, and allocates memory
 * for the value. The entry contains the provided key and value with the specified size.
 *
 * @param key Null-terminated string representing the key of the entry.
 * @param value Pointer to the value associated with the key.
 * @param size Size in bytes of the value.
 * @return Pointer to the newly created hash table entry.
 */
static HT_Entry *HT_NewEntry(const char *key, const void *value, const size_t size) {
    HT_Entry *entry = malloc(sizeof(HT_Entry));
    if (!entry) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }

    const HT_Entry placeholder = {.key = strdup(key), .value = malloc(size), .size = size};
    memcpy(entry, &placeholder, sizeof(HT_Entry));
    if (!entry->value) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }
    memcpy((void *) entry->value, value, size);
    return entry;
}

/**
 * @brief Deletes a hash table entry, freeing memory for the key and value.
 *
 * @param entry Pointer to the hash table entry to delete.
 */
static void HT_DeleteEntry(HT_Entry *entry) {
    free((void *) entry->key);
    free((void *) entry->value);
    free(entry);
}

/**
 * @brief Finds the next prime number greater than or equal to the specified number.
 *
 * This function ensures that the hash table capacity is set to a prime number
 * to improve distribution of entries. If the input number is less than 4, returns 3.
 *
 * @param x The starting number to check for a prime.
 * @return The next prime number greater than or equal to x.
 */
static size_t HT_NextPrime(size_t x) {
    if (x < 4)
        return 3;
    if (x % 2 == 0)
        ++x;
start:
    const double upper = floor(sqrt((double) x));
    for (int i = 3; i <= upper; i += 2) {
        if (x % i == 0) {
            x += 2;
            goto start;
        }
    }
    return x;
}

HT_HashTable *HT_New(const size_t capacity) {
    HT_HashTable *ht = malloc(sizeof(HT_HashTable));
    if (!ht) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }
    ht->capacity = capacity;
    ht->buckets = HT_NextPrime(capacity);
    ht->count = 0;
    ht->entries = calloc(ht->buckets, sizeof(HT_Entry *));
    if (!ht->entries) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }
    return ht;
}

void HT_Delete(HT_HashTable *ht) {
    for (size_t i = 0; i < ht->count; ++i) {
        HT_Entry *entry = ht->entries[i];
        if (!HT_IsNullOrDeleted(entry)) {
            HT_DeleteEntry(entry);
        }
    }
    free(ht->entries);
    free(ht);
}

/**
 * @brief Resizes the hash table to the specified capacity.
 *
 * Allocates a new hash table with the given capacity, rehashes all existing
 * entries into the new table, and then replaces the current table with the new one.
 *
 * @param ht Pointer to the hash table to resize.
 * @param capacity The new capacity for the resized hash table.
 */
static void HT_ResizeHashTable(HT_HashTable *ht, size_t capacity) {
    if (capacity < HT_DEFAULT_CAPACITY) {
        capacity = HT_DEFAULT_CAPACITY;
    }

    HT_HashTable *new_ht = HT_New(capacity);
    for (size_t i = 0; i < ht->buckets; ++i) {
        HT_Entry *entry = ht->entries[i];
        if (!HT_IsNullOrDeleted(entry)) {
            HT_Put(new_ht, entry->key, entry->value, entry->size);
        }
    }

    ht->capacity = new_ht->capacity;
    ht->count = new_ht->count;

    const size_t buckets = ht->buckets;
    ht->buckets = new_ht->buckets;
    new_ht->buckets = buckets;

    HT_Entry **temp_entries = ht->entries;
    ht->entries = new_ht->entries;
    new_ht->entries = temp_entries;

    HT_Delete(new_ht);
}

/**
 * @brief Generates a hash value for a given key using a specified prime number.
 *
 * This function calculates a hash by iterating over the key's characters and
 * raising the prime to different powers, modulating the result by the specified modulus.
 *
 * @param key Null-terminated string to hash.
 * @param mod The modulus for the resulting hash.
 * @param prime Prime number used in the hash calculation.
 * @return An integer hash value for the key, modulated by the provided modulus.
 */
static int HT_HashFunction(const char *key, const size_t mod, const int prime) {
    size_t hash = 0;
    const size_t len = strlen(key);
    for (size_t i = 0; i < len; ++i) {
        hash += (size_t) pow(prime, (double) (len - (i + 1) * key[i]));
    }
    return (int) (hash % mod);
}

/**
 * @brief Computes the index for a key in the hash table using double hashing.
 *
 * Uses two hash functions to calculate a unique index for each attempt,
 * ensuring that collisions are resolved by using a secondary hash function.
 *
 * @param key Null-terminated string representing the key to locate.
 * @param buckets The number of buckets (size of the hash table).
 * @param attempt The current attempt number for resolving a collision.
 * @return The computed index within the range of available buckets.
 */
static int HT_GetKeyIndex(const char *key, const size_t buckets, const int attempt) {
    const int hash_a = HT_HashFunction(key, buckets, HT_PRIME_1);
    int hash_b = HT_HashFunction(key, buckets, HT_PRIME_2);
    if (hash_a % buckets == 0) {
        hash_b = 1;
    }
    return (int) ((hash_a + hash_b * attempt) % buckets);
}

const void *HT_Get(const HT_HashTable *ht, const char *key) {
    HT_Entry *entry;
    int attempt = 0;
    do {
        const int index = HT_GetKeyIndex(key, ht->buckets, attempt);
        entry = ht->entries[index];
        if (!HT_IsNullOrDeleted(entry) && strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        ++attempt;
    } while (entry);

    return nullptr;
}

void HT_Put(HT_HashTable *ht, const char *key, const void *value, const size_t size) {
    if (ht->count * 100 / ht->buckets > 70) {
        HT_ResizeHashTable(ht, ht->capacity * 2);
    }

    HT_Entry *new = HT_NewEntry(key, value, size);
    HT_Entry *old;
    int i = 0;
    int index = 0;
    do {
        index = HT_GetKeyIndex(key, ht->buckets, i);
        old = ht->entries[index];
        if (!HT_IsNullOrDeleted(old) && strcmp(old->key, key) == 0) {
            HT_DeleteEntry(old);
            ht->entries[index] = new;
            return;
        }
        ++i;
    } while (!HT_IsNullOrDeleted(old));
    ht->entries[index] = new;
    ht->count++;
}

bool HT_Remove(HT_HashTable *ht, const char *key) {
    if (ht->count * 100 / ht->buckets < 30) {
        HT_ResizeHashTable(ht, ht->capacity / 2);
    }

    HT_Entry *entry;
    int i = 0;
    do {
        const int index = HT_GetKeyIndex(key, ht->buckets, i);
        entry = ht->entries[index];
        if (!HT_IsNullOrDeleted(entry) && strcmp(entry->key, key) == 0) {
            HT_DeleteEntry(entry);
            ht->entries[index] = &DELETED_ENTRY;
            ht->count--;
            return true;
        }
        ++i;
    } while (entry);

    return false;
}
