#include <stdio.h>
#include <stdlib.h>
#include "hash_map.h"
#include "connect_4_helpers.h"

// int continue_game;
// char board[6][7];
// int directions[4][2] = {{-1, 0}, {0, -1}, {-1, -1}, {-1, 1}};
// void play_game();

int check_winner(char board[6][7], char player) {
    LinkedList* vertical = map();
    LinkedList* horizontal = map();
    LinkedList* pos_diagonal = map();
    LinkedList* neg_diagonal = map();
    LinkedList** dictionaries = (LinkedList **)malloc(sizeof(vertical) * 4);
    dictionaries[0] = vertical;
    dictionaries[1] = horizontal;
    dictionaries[2] = pos_diagonal;
    dictionaries[3] = neg_diagonal;
    int directions[4][2] = {{1, 0}, {0, -1}, {1, -1}, {1, 1}};

    for (int i = 5; i > -1; i--) {
        for (int j = 0; j < 7; j++) {
            int coord[2] = {i, j};
            if (board[i][j] == player) {
                printf("here\n");
                for (int k = 0; k < 4; k++){
                    int prev[2] = {i + directions[k][0], j + directions[k][1]};
                    if (contains(prev, dictionaries[k]) == 1){
                        printf("Here2/n");
                        int count = get(prev, dictionaries[k]);
                        if (count == 3) {
                            free(dictionaries);
                            return 1;
                        }
                        else {
                            add(coord, count + 1, dictionaries[k]);
                        }
                    }
                    else {
                        add(coord, 1, dictionaries[k]);
                        printf("contains i: %d j: %d: c:%d\n",i, j, contains(coord, dictionaries[k]));
                    }
                }
                
            }
            
        } 
    }
    free(dictionaries);
    return 0;
}

void play_game() {

}