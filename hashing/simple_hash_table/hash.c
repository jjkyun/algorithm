#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

void InitHashTable(HashTable* ht){
    int i;
    for(i = 0; i < ht->size; i++){
        // Initalize hashtable status to be empty 
        ht->arr[i].status = EMPTY;
    }
}

HashTable* makeHashTable(int size)
{
    int i;
    if(size > 0){
        HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
        ht->size = size * 3;  // Hash Table size == size of input data * 3
        ht->arr = (HashNode*)malloc(sizeof(HashNode) * ht->size);
        
        // Initialize hashtable
        InitHashTable(ht);
        return ht;
    } else {    
        printf("Hash Table size Error..!");
        return NULL;
    }
}

void insertHashNode(HashTable* ht, int key, int value)
{
    int hash_index;
    int i;
    // Find Hash index through linear probing method
    hash_index = linearProbing(ht, key);
    // Insert the data into Hash Table
    ht->arr[hash_index].key = key;
    ht->arr[hash_index].value = value;
    ht->arr[hash_index].status = FULL;
}

HashNode* findHashNode(HashTable* ht, int key)
{
    // Convert the key through hash function
    int i = key % ht->size;
    // Loop until status is FULL or DELETED
    while(ht->arr[i].status != EMPTY){
        if(ht->arr[i].key == key && ht->arr[i].status == FULL){
            // Return hashnode when same key found
            return &ht->arr[i];      
        }
        i = ++i % ht->size;
    }
    // Not found 
    return NULL;
}

void deleteHashNode(HashTable* ht, int key)
{
    HashNode* node;
    // Find hash node
    node = findHashNode(ht, key);    
    if(node != NULL){
        // Make status DELETED
        node->status = DELETED;
    }
}

int linearProbing(HashTable* ht, int key)
{
    // Hash function
    int i = key % ht->size;
    // Check if status is EMPTY or DELETED
    // If it is not FULL, return the index
    if(ht->arr[i].status != FULL){
        return i;
    } else {
        // Keep search until EMPTY or DELETED status is find
        while(ht->arr[i].status != EMPTY && ht->arr[i].status != DELETED){
            i = ++i % ht->size;
        }
        return i;
    }
}
