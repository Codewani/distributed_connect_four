#include <stdio.h>
#include <stdlib.h>
#include "util/hash_map.h"
#include "connect_4_helpers.h"

// int continue_game;
// char board[6][7];
// int directions[4][2] = {{-1, 0}, {0, -1}, {-1, -1}, {-1, 1}};
// void play_game();

LinkedList* bottom;
LinkedList* left;
LinkedList* top_left;
LinkedList* top_right;
LinkedList* bottom_left;
LinkedList* right;
LinkedList* bottom_right;
LinkedList** dir_dictionaries;
LinkedList* valid_spaces;
int directions[7][2] = {{-1, 0}, {0, -1}, {1, -1}, {1, 1}, {-1, -1}, {0, 1}, {-1, 1}};

void initialize(void) {
    bottom = hash_map();
    left = hash_map();
    top_left = hash_map();
    top_right = hash_map();
    bottom_left = hash_map();
    right = hash_map();
    bottom_right = hash_map();
    valid_spaces = hash_map();
    
    dir_dictionaries = (LinkedList **)malloc(sizeof(* dir_dictionaries) * 7);
    dir_dictionaries[0] = bottom;
    dir_dictionaries[1] = left;
    dir_dictionaries[2] = top_left;
    dir_dictionaries[3] = top_right;
    dir_dictionaries[4] = bottom_left;
    dir_dictionaries[5] = right;
    dir_dictionaries[6] = bottom_right;
    
    for (int j = 0; j < 7; j++) {
        int valid_pos[2] = {5, j};
        add(valid_pos, 0, valid_spaces);
    }
}

int is_valid_move(int i, int j) {
    int coord[2] = {i, j};
    return contains(coord, valid_spaces);
}

void make_move(int i, int j, char player) {
    int coord[2] = {i, j};
    remove_item(coord, valid_spaces);
    if (i > 0) {
        int new_valid[2] = {i - 1, j};
        add(new_valid, 0, valid_spaces);
    }
}

int check_winner(char board[6][7], char player) {

    for (int i = 5; i > -1; i--) {
        for (int j = 0; j < 7; j++) {
            int coord[2] = {i, j};
            if (board[i][j] == player) {
                for (int k = 0; k < 7; k++){
                    int prev[2] = {i + directions[k][0], j + directions[k][1]};
                    if (contains(prev, dir_dictionaries[k]) == 1){
                        int count = get(prev, dir_dictionaries[k]);
                        if (count == 3) {
                            free(dir_dictionaries);
                            return 1;
                        }
                        else {
                            add(coord, count + 1, dir_dictionaries[k]);
                        }
                    }
                    else {
                        add(coord, 1, dir_dictionaries[k]);
                        printf("contains i: %d j: %d: c:%d\n",i, j, contains(coord, dir_dictionaries[k]));
                    }
                }
                
            }
            
        } 
    }
    free(dir_dictionaries);
    return 0;
}

/* The interactive game loop and main were removed so this file provides
   the connect-4 helper functions (initialize, is_valid_move, make_move,
   check_winner) that can be used by a networked server. */