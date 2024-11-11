#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "hash_table.h"

int main(void) {
    const char* key1 = "key1";
    const char* key2 = "key2";
    const char* key3 = "key3";
    const char* key4 = "key4";
    const char* key5 = "key5";
    const char* value1 = "value1";
    const char* value2 = "value2";
    const char* value3 = "value3";
    const char* value4 = "value4";

    HT_HashTable* ht = HT_NewHashTable(53);

    printf("Put [%s] = %s\n", key1, value1);
    HT_Put(ht, key1, value1, strlen(value1));
    printf("Put [%s] = %s\n", key2, value2);
    HT_Put(ht, key2, value2, strlen(value2));
    printf("Put [%s] = %s\n", key3, value3);
    HT_Put(ht, key3, value3, strlen(value3));
    printf("Put [%s] = %s\n", key4, value4);
    HT_Put(ht, key4, value4, strlen(value4));

    printf("Get [%s] = %s\n", key1, (const char*)HT_Get(ht, key1));
    printf("Get [%s] = %s\n", key2, (const char*)HT_Get(ht, key2));
    printf("Get [%s] = %s\n", key3, (const char*)HT_Get(ht, key3));
    printf("Get [%s] = %s\n", key4, (const char*)HT_Get(ht, key4));
    printf("Get [%s] = %s\n", key5, (const char*)HT_Get(ht, key5));

    printf("Remove [%s] = %s\n", key1, HT_Remove(ht, key1) ? "true" : "false");
    printf("Remove [%s] = %s\n", key2, HT_Remove(ht, key2) ? "true" : "false");
    printf("Remove [%s] = %s\n", key3, HT_Remove(ht, key3) ? "true" : "false");
    printf("Remove [%s] = %s\n", key4, HT_Remove(ht, key4) ? "true" : "false");
    printf("Remove [%s] = %s\n", key5, HT_Remove(ht, key5) ? "true" : "false");

    HT_DeleteHashTable(ht);
    return 0;
}
