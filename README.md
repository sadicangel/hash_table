# hash_table

<img src=icon.png width=128></img>  
An implementation of a hash table in C, supporting dynamic resizing, collision handling through double hashing, and efficient memory management.
The hash table is designed for general-purpose key-value storage, with customizable options for entry capacity and load factor management.

The primary API functions provided by `HT_HashTable` are:

- `HT_HashTable *HT_New(size_t capacity)`: Initializes a new hash table with a specified capacity.
- `void HT_Delete(HT_HashTable *ht)`: Frees all memory associated with the hash table.
- `const void *HT_Get(const HT_HashTable *ht, const char *key)`: Retrieves a value by key.
- `void HT_Put(HT_HashTable *ht, const char *key, const void *value, size_t size)`: Inserts or updates a key-value pair in the hash table.
- `bool HT_Remove(HT_HashTable *ht, const char *key)`: Removes an entry by key.

## Example

```c
#include "hash_table.h"
#include <stdio.h>

int main() {
    HT_HashTable *ht = HT_New(4);
    int value = 42;

    HT_Put(ht, "example_key", &value, sizeof(value));
    
    const int *retrieved_value = HT_Get(ht, "example_key");
    if (retrieved_value) {
        printf("Retrieved value: %d\n", *retrieved_value);
    }

    HT_Remove(ht, "example_key");
    HT_Delete(ht);

    return 0;
}
```

## References

- [Hash table](https://en.wikipedia.org/wiki/Hash_table)
- [Write a hash table in C](https://github.com/jamesroutley/write-a-hash-table)