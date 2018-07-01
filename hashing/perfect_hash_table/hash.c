#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "hash.h"

// Make hash function randomly
// Linear congruential generator:((a*x + b) mod p) mod m
int* generateRandomFactor(int size){
    // a, b, p
    int* factor = (int*)malloc(sizeof(int) * 4);
    factor[2]= pow(2, 8) + 1; // p(prime) should be (power of power of 2) + 1
    factor[0] = rand() % factor[2]; // a less then p
    factor[1] = rand() % factor[2]; // b less then p
    factor[3] = size;
    return factor;
}

// Hash function converting to hash key
int hashFunction(int inputKey, int* factor){
    return ((factor[0] * inputKey + factor[1]) % factor[2]) % factor[3];
}

// 2-level hash function
int hashFunction2(HashTable* ht, int key){
    int i = (137 * key) % ht->size;
    // Check if status is FULL
    if(ht->arr[i].status == FULL){
        // Then linearprobing
        while(ht->arr[i].status != EMPTY && ht->arr[i].status != DELETED){
            i = ++i % ht->size;
        }
        return i;
    } else {
        return i;
    }
}

// Count number of keys in entry 
void countCollision(int* inputKeys, HashTable* ht, int* factor){
    int i, hash_index;
    // Count the hash keys 
    for(i = 0; i < ht->size; i++){
        hash_index = hashFunction(inputKeys[i], factor);
        ht->arr[hash_index].count++;
    }
}   

void InitHashTable(HashTable* ht){
    int i;
    for(i = 0; i < ht->size; i++){
        ht->arr[i].count = 0;
        ht->arr[i].second_ht = NULL;
        ht->arr[i].status = EMPTY;
    }   
}

HashTable* makeHashTable(int* inputKeys, int size, int* factor)
{
    if(size > 0){
        int i;
        // Make first hash table
        HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
        ht->size = size; // Hash table size == size of keys
        ht->arr = (HashNode*)malloc(sizeof(HashNode) * ht->size);
        
        // Initiate hash table
        InitHashTable(ht);
       
        // Count the collision
        countCollision(inputKeys, ht, factor);

        // Make second hash table
        for(i = 0; i < ht->size; i++){
            // Check collision count
            // If more than 1 collision, Make second hash table
            if(ht->arr[i].count >= 2){
                ht->arr[i].second_ht = 
                    (HashTable*)malloc(sizeof(HashTable));
                // Second hash table size is square of count
                ht->arr[i].second_ht->size = (ht->arr[i].count-1) * (ht->arr[i].count-1);
                ht->arr[i].second_ht->arr = 
                    (HashNode*)malloc(sizeof(HashNode) * ht->arr[i].second_ht->size);
                InitHashTable(ht->arr[i].second_ht);
            }
        }
        return ht;
    } else {
        printf("Hash Table size Error..!");
        return NULL;
    }
}

void insertHashNode(HashTable* ht, int key, int value, int* factor)
{
    int i, j;
    // Find Hash index through hash function
    i = hashFunction(key, factor);
    // Check if there is already data
    if(ht->arr[i].status == FULL){
        // If there is, then go to second hash table using linear probing
        // Find the index in second hash table
        j = hashFunction2(ht->arr[i].second_ht, key);
        ht->arr[i].second_ht->arr[j].key = key;
        ht->arr[i].second_ht->arr[j].value = value;
        ht->arr[i].second_ht->arr[j].status = FULL;
    
    } else {
        ht->arr[i].key = key;
        ht->arr[i].value = value;
        ht->arr[i].status = FULL;
    }
}

HashNode* findHashNode(HashTable* ht, int key, int* factor) 
{
    int j;
    // Get hash key
    int i = hashFunction(key, factor);
    
    // Search in first hash table
    if (ht->arr[i].key == key && ht->arr[i].status == FULL){
        return &ht->arr[i];
    // Search in second hash table
    } else if (ht->arr[i].second_ht != NULL){
        // Loop until status is FULL or DELETED
        for (j = 0; j < ht->arr[i].second_ht->size; j++) {
            if (ht->arr[i].second_ht->arr[j].key == key && 
                    ht->arr[i].second_ht->arr[j].status == FULL) {
                return &ht->arr[i].second_ht->arr[j];
            }
        }
        return NULL;
    }
}

void deleteHashNode(HashTable* ht, int key, int* factor)
{
    HashNode* node;
    node = findHashNode(ht, key, factor);
    if (node != NULL) { if (node->key == key) { node->status = DELETED;
        }
    }
}

void printHashTable(HashTable* ht){
    int i, j;
    for (i = 0; i < ht->size; i ++) {
        printf("1: [key: %d, value: %d] ", ht->arr[i].key, ht->arr[i].value);
        if (ht->arr[i].second_ht != NULL) {
            for (j = 0; j < ht->arr[i].second_ht->size ; j++) {
                printf("2: [key: %d, value %d] ", ht->arr[i].second_ht->arr[j].key, ht->arr[i].second_ht->arr[j].value);
            }
        } else {
            printf("  NO Second Table");
        }
        printf("\n");
    }
}


