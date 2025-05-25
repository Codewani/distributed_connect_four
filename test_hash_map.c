#include <stdio.h>
#include "hash_map.h"
#include <assert.h>
#include <stdlib.h>

int main(int argc, char const* argv[]) {
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
    LinkedList** dictionaries = (LinkedList **)malloc(sizeof(vertical) * 4);
    dictionaries[0] = vertical;
    dictionaries[1] = horizontal;
    dictionaries[2] = pos_diagonal;
    dictionaries[3] = neg_diagonal;
    int coord[2] = {1, 2};
    add(coord, 1, dictionaries[0]);
    assert(contains(coord, vertical));
    assert(!contains(coord, horizontal));


    printf("All tests passed!\n");
    return 0;
}