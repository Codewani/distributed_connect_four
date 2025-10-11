#include <stdio.h>
#include <stdlib.h>
#include "hash_map.h"
#include "connect_4_helpers.h"

LinkedList* vertical;
LinkedList* horizontal;
LinkedList* pos_diagonal;
LinkedList* neg_diagonal;
LinkedList** dir_dictionaries;
LinkedList* valid_spaces;
int directions[4][2] = {{1, 0}, {0, -1}, {1, -1}, {1, 1}};

// void initialize(void) {
//     vertical = map();
//     horizontal = map();
//     pos_diagonal = map();
//     neg_diagonal = map();
//     valid_spaces = map();
//     dir_dictionaries = (LinkedList **)malloc(sizeof(vertical) * 4);
//     dir_dictionaries[0] = vertical;
//     dir_dictionaries[1] = horizontal;
//     dir_dictionaries[2] = pos_diagonal;
//     dir_dictionaries[3] = neg_diagonal;
//     for (int j = 0; j < 7; j++) {
//         int valid_pos[2] = {5, j};
//         add(valid_pos, 0, valid_spaces);
//     }
// }
// int is_valid_move(int i, int j) {
//     int coord[2] = {i, j};
//     return contains(coord, valid_spaces);
// }

// void make_move(int i, int j, char player) {
//     int coord[2] = {i, j};
//     remove_item(coord, valid_spaces);
//     if (i > 0) {
//         int new_valid[2] = {i - 1, j};
//         add(new_valid, 0, valid_spaces);
//     }
// }