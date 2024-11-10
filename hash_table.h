//
// Created by sadic on 10/11/2024.
//

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct {
    char* key;
    void* value;
} HT_Entry;

typedef struct {
    int count;
    int capacity;
    HT_Entry** entries;
} HT_HashTable;

HT_HashTable* HT_NewHashTable(int capacity);
void HT_DeleteHashTable(HT_HashTable* ht);

#endif //HASH_TABLE_H
