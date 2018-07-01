#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

enum STATUS {EMPTY, DELETED, FULL};

// Hash node type
typedef struct HashNode {
    int key;
    int value;
    enum STATUS status; // status for empty, deleted, full
} HashNode;

// Hashtable type
typedef struct HashTable {
    int size;
    HashNode* arr;
} HashTable;

// Hashing function
void InitHashTable(HashTable* ht);
HashTable* makeHashTable(int size);
void insertHashNode(HashTable* ht, int key, int value);
HashNode* findHashNode(HashTable* ht, int key);
void deleteHashNode(HashTable* ht, int key);
int linearProbing(HashTable* ht, int key);

#endif /* __HASHTABLE_H__ */
