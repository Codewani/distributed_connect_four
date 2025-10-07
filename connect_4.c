#include <stdio.h>
#include <stdlib.h>
#include "util/hash_map.h"
#include "connect_4_helpers.h"

// int continue_game;
// char board[6][7];
// int directions[4][2] = {{-1, 0}, {0, -1}, {-1, -1}, {-1, 1}};
// void play_game();

LinkedList* vertical;
LinkedList* horizontal;
LinkedList* pos_diagonal;
LinkedList* neg_diagonal;
LinkedList** dir_dictionaries;
LinkedList* valid_spaces;
int directions[4][2] = {{-1, 0}, {0, -1}, {1, -1}, {1, 1}, {-1, -1}, {0, 1}, {-1, 1}};

void initialize(void) {
    vertical = map();
    horizontal = map();
    pos_diagonal = map();
    neg_diagonal = map();
    valid_spaces = map();
    dir_dictionaries = (LinkedList **)malloc(sizeof(vertical) * 4);
    dir_dictionaries[0] = vertical;
    dir_dictionaries[1] = horizontal;
    dir_dictionaries[2] = pos_diagonal;
    dir_dictionaries[3] = neg_diagonal;
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
                for (int k = 0; k < 4; k++){
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

void play_game() {
    int player_x = 1;
    int player_o = 0;
    int current_player = player_x;
    char position[32];
    char board[6][7];
    initialize();

    while (1) {
        printf("Enter a position as in '(x, y)': ");
        if (fgets(position, sizeof(position), stdin) == NULL) {
            break; // EOF or error
        }
        int x, y;
        /* parse input like: (4, 0) - allow spaces */
        if (sscanf(position, " ( %d , %d ) ", &x, &y) != 2) {
            printf("Invalid input. Please enter like (4, 0)\n");
            continue;
        }
        int coord[2] = {x, y};
        for (int k = 0; k < 4; k++){
            int prev[2] = {x + directions[k][0], y + directions[k][1]};
            if (contains(prev, dir_dictionaries[k]) == 1){
                printf("Here2/n");
                int count = get(prev, dir_dictionaries[k]);
                if (count == 3) {
                    free(dir_dictionaries);
                    printf("There is a winner");
                    break;
                }
                else {
                    add(coord, count + 1, dir_dictionaries[k]);
                }
            }
            else {
                printf("Here1");
                add(coord, 1, dir_dictionaries[k]);
            }
        }

    }

}

int main(int argc, char const* argv[])
{
    play_game();
}