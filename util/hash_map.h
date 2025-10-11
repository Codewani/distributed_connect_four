#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stddef.h>

#define HASH_SET_SIZE 10000

// Data structure definitions
typedef struct LinkedList {
    int key[2];
    int value;
    struct LinkedList* next;
} LinkedList;

// Global variable declaration
extern LinkedList* buckets;

// Function declarations
LinkedList* hash_map(void);
int hash_function(int* key);
void add(int* key, int value, LinkedList* buckets);
void put(int* key, int value, LinkedList* buckets);
void remove_item(int* key, LinkedList* buckets);
int contains(int* key, LinkedList* buckets);
int get(int* key, LinkedList* buckets);

#endif // HASH_SET_H