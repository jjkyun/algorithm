#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

enum STATUS {EMPTY, DELETED, FULL};

typedef struct HashTable HashTable;

//  Hash node type
typedef struct HashNode {
    int key;
    int value;
    int count; // Collision count 
    HashTable* second_ht; // Second hash table for collision
    enum STATUS status;
} HashNode;

// hash table
typedef struct HashTable {
    int size;
    HashNode* arr;
} HashTable;


/* Support those below interfaces (used in main.c) */
int* generateRandomFactor(int size);
int hashFunction(int inputKeys, int* factor);
int hashFunction2(HashTable* ht, int key);
void countCollision(int* inputKeys, HashTable* ht, int* factor);
void InitHashTable(HashTable* ht);
HashTable* makeHashTable(int* inputKeys, int size, int* factor);
void insertHashNode(HashTable* ht, int key, int value, int* factor);
HashNode* findHashNode(HashTable* ht, int key, int* factor);
void deleteHashNode(HashTable* ht, int key, int* factor);
void printHashTable(HashTable* ht);

#endif /* __HASHTABLE_H__ */
