#include <stdio.h>
#include "../util/hash_map.h"
#include <assert.h>
#include <stdlib.h>

int main(int argc, char const* argv[]) {
    printf("Running tests for hash_map...\n");
    LinkedList* my_dict = map();
    LinkedList* my_dict2 = map();
    int array1[2] = {1, 2};
    int array2[2] = {1, 2};
    int array3[2] = {2, 1};
    add(array1, 0, my_dict);
    assert(contains(array2, my_dict));
    assert(contains(array1, my_dict));
    remove_item(array2, my_dict);
    add(array1, 0, my_dict2);
    assert(!contains(array1, my_dict));
    assert(!contains(array2, my_dict));
    assert(contains(array2, my_dict2));
    assert(contains(array1, my_dict2));
    assert(get(array1, my_dict2) == 0);
    assert(get(array2, my_dict2) == 0);
    add(array2, 5, my_dict2);
    assert(get(array1, my_dict2) == 5);
    assert(get(array2, my_dict2) == 5);

    LinkedList* vertical = map();
    LinkedList* horizontal = map();
    LinkedList* pos_diagonal = map();
    LinkedList* neg_diagonal = map();
    LinkedList** dir_dictionaries = (LinkedList **)malloc(sizeof(vertical) * 4);
    dir_dictionaries[0] = vertical;
    dir_dictionaries[1] = horizontal;
    dir_dictionaries[2] = pos_diagonal;
    dir_dictionaries[3] = neg_diagonal;
    int coord[2] = {1, 2};
    add(coord, 1, dir_dictionaries[0]);
    assert(contains(coord, vertical));
    assert(!contains(coord, horizontal));

    // Test empty map
    LinkedList* empty_map = map();
    int test_key[2] = {5, 5};
    assert(!contains(test_key, empty_map));

    // Test negative coordinates
    LinkedList* neg_map = map();
    int neg_coord[2] = {-1, -2};
    add(neg_coord, 10, neg_map);
    assert(contains(neg_coord, neg_map));
    assert(get(neg_coord, neg_map) == 10);

    // Test overwriting values
    LinkedList* overwrite_map = map();
    int key[2] = {3, 4};
    add(key, 100, overwrite_map);
    assert(get(key, overwrite_map) == 100);
    add(key, 200, overwrite_map);
    assert(get(key, overwrite_map) == 200);

    // Test multiple keys with same hash
    LinkedList* collision_map = map();
    int key1[2] = {0, 0};
    int key2[2] = {HASH_SET_SIZE, 0};
    add(key1, 1, collision_map);
    add(key2, 2, collision_map);
    assert(get(key1, collision_map) == 1);
    assert(get(key2, collision_map) == 2);

    // Test remove non-existent key
    LinkedList* remove_map = map();
    int nonexist[2] = {99, 99};
    remove_item(nonexist, remove_map);
    assert(!contains(nonexist, remove_map));

    printf("All tests passed!\n");
    return 0;
}