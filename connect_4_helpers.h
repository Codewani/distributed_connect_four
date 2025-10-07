#ifndef CONNECT_4_H
#define CONNECT_4_H
#include "util/hash_map.h"
int check_winner(char board[6][7], char player);
void initialize(void);
int is_valid_move(int i, int j);

#endif