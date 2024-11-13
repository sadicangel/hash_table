#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "hash_table.h"

int main(void) {
    HT_HashTable* ht = HT_New(HT_DEFAULT_CAPACITY);
    for (int i = 0; i < 50; ++i) {
        char key[50], value[50];
        sprintf(key, "key-%d", i);
        sprintf(value, "value-%d", i);
        HT_Put(ht, key, value, strlen(value));

        printf("count: %llu, buckets: %llu\n", ht->count, ht->buckets);
    }
    for (int i = 0; i < 50; ++i) {
        char key[50];
        sprintf(key, "key-%d", i);
        HT_Remove(ht, key);
        printf("count: %llu, buckets: %llu\n", ht->count, ht->buckets);
        if(ht->count == 5)
            {
            printf("success\n");
        }
    }
    HT_Delete(ht);

    return 0;
}
