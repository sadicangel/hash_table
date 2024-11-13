//
// Created by sadic on 10/11/2024.

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>
#include <stdbool.h>

/**
 * @def HT_DEFAULT_CAPACITY
 * The default initial capacity for a new hash table.
 */
#define HT_DEFAULT_CAPACITY 4

/**
 * @struct HT_Entry
 * @brief Represents a key-value pair within a hash table.
 *
 * This structure holds the key and associated value, as well as the size of the value.
 */
typedef struct {
    const char *key;     /**< The key for the entry, as a null-terminated string. */
    const void *value;   /**< Pointer to the value associated with the key. */
    size_t size;         /**< Size in bytes of the value. */
} HT_Entry;

/**
 * @struct HT_HashTable
 * @brief Represents a hash table.
 *
 * This structure maintains an array of entries, a capacity, and a count of the
 * current entries.
 */
typedef struct {
    size_t capacity;     /**< Total capacity of the hash table. */
    size_t buckets;      /**< Number of buckets in the hash table. */
    size_t count;        /**< Current count of entries in the hash table. */
    HT_Entry **entries;  /**< Array of pointers to hash table entries. */
} HT_HashTable;

/**
 * @brief Creates a new hash table with the specified initial capacity.
 *
 * Allocates and initializes a new hash table with the given capacity, or with
 * the default capacity if no value is provided.
 *
 * @param capacity Initial number of entries that the hash table can contain.
 *                 If zero, defaults to @ref HT_DEFAULT_CAPACITY.
 * @return A pointer to a newly created hash table, or NULL if allocation fails.
 */
HT_HashTable *HT_New(size_t capacity);

/**
 * @brief Deletes the specified hash table.
 *
 * Frees all memory associated with the hash table, including all entries.
 *
 * @param ht Pointer to the hash table to delete. If NULL, the function does nothing.
 */
void HT_Delete(HT_HashTable *ht);

/**
 * @brief Retrieves a value from the hash table by key.
 *
 * Searches for an entry matching the specified key and returns the associated
 * value, or NULL if the key is not found.
 *
 * @param ht Pointer to the hash table.
 * @param key The key of the entry to find.
 * @return Pointer to the value associated with the key, or NULL if the key does not exist.
 */
const void *HT_Get(const HT_HashTable *ht, const char *key);

/**
 * @brief Inserts or updates an entry in the hash table.
 *
 * If the specified key already exists, its value is updated. Otherwise, a new entry is added.
 *
 * @param ht Pointer to the hash table.
 * @param key The key of the entry to add or update.
 * @param value Pointer to the value associated with the key.
 * @param size The size of the value in bytes.
 */
void HT_Put(HT_HashTable *ht, const char *key, const void *value, size_t size);

/**
 * @brief Removes an entry from the hash table by key.
 *
 * Deletes the entry with the specified key if it exists in the hash table.
 *
 * @param ht Pointer to the hash table.
 * @param key The key of the entry to remove.
 * @return True if the entry was found and removed, false otherwise.
 */
bool HT_Remove(HT_HashTable *ht, const char *key);

#endif // HASH_TABLE_H
