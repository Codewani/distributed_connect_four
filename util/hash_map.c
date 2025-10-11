#include "hash_map.h"
#include <stdlib.h>

LinkedList* buckets;

LinkedList* hash_map(void) {
    buckets = (LinkedList *)malloc(sizeof(LinkedList) * HASH_SET_SIZE);
    for(size_t i = 0; i < HASH_SET_SIZE; i++) {
        buckets[i].next = NULL;
    }
    return buckets;
}

int hash_function(int* key) {
    return (key[0] * 51 + key[1] * 13) % HASH_SET_SIZE;
}

void add(int* key, int value, LinkedList* buckets) {
    if (contains(key, buckets)) {
        put(key, value, buckets);
        return;
    }
    int index = hash_function(key);
    LinkedList* cur = &buckets[index];
    
    LinkedList* new_node = (LinkedList*)malloc(sizeof(LinkedList));
    new_node->key[0] = key[0];
    new_node->key[1] = key[1];
    new_node->value = value;
    new_node->next = NULL;

    while (cur->next) {
        cur = cur->next;
    }
    cur->next = new_node;
}

void remove_item(int* key, LinkedList* buckets) {
    int index = hash_function(key);
    LinkedList* cur = &buckets[index];
    LinkedList* nxt = cur->next;

    while (nxt && (nxt->key[0] != key[0] || nxt->key[1] != key[1])) {
        cur = nxt;
        nxt = nxt->next;
    }

    if (nxt) {
        cur->next = nxt->next;
        free(nxt);
    }
}

int contains(int* key, LinkedList* buckets) {
    int index = hash_function(key);
    LinkedList* cur = &buckets[index];
    cur = cur->next;

    while (cur) {
        if (cur->key[0] == key[0] && cur->key[1] == key[1]) {
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}
void put(int* key, int value, LinkedList* buckets) {
    int index = hash_function(key);
    LinkedList* cur = &buckets[index];
    cur = cur->next;

    while (cur) {
        if (cur->key[0] == key[0] && cur->key[1] == key[1]) {
            cur->value = value;
        }
        cur = cur->next;
    }
}
int get(int* key, LinkedList* buckets) {
    int index = hash_function(key);
    LinkedList* cur = &buckets[index];
    cur = cur->next;

    while (cur) {
        if (cur->key[0] == key[0] && cur->key[1] == key[1]) {
            return cur->value;
        }
        cur = cur->next;
    }
    return -1;
}