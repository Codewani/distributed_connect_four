#include "connect_4_helpers.h"
#include <stdio.h>
#include <assert.h>

int main(int argc, char const* argv[]) {
    char board[6][7];

    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 7; c++) {
            board[r][c] = 'E';
        }
    }
    
    board[5][0] = 'X';
    board[4][1] = 'X';
    board[3][2] = 'X';
    board[2][3] = 'X';

    board[5][1] = 'O';
    board[5][2] = 'O';
    board[5][3] = 'O';
    check_winner(board, 'X');
    assert(check_winner(board, 'X') == 1);
    assert(check_winner(board, 'O') == 0);

    printf("All tests passed!\n");
    return 0;
}