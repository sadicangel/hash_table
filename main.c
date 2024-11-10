#include <stdio.h>

#include "hash_table.h"

int main(void) {
    HT_HashTable* ht = HT_NewHashTable(53);
    HT_DeleteHashTable(ht);
    return 0;
}
