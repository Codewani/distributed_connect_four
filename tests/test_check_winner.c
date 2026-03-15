#include "../connect_4_helpers.h"
#include <stdio.h>
#include <assert.h>

void clear_board(char board[6][7]) {
    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 7; c++) {
            board[r][c] = 'E';
        }
    }
}

int main(int argc, char const* argv[]) {
    char board[6][7];

    // Test 1: Diagonal win (original test)
    clear_board(board);
    board[5][0] = 'X';
    board[4][1] = 'X';
    board[3][2] = 'X';
    board[2][3] = 'X';
    board[5][1] = 'O';
    board[5][2] = 'O';
    board[5][3] = 'O';
    assert(check_winner(board, 'X') == 1);
    assert(check_winner(board, 'O') == 0);
    printf("Test 1 passed: Diagonal win\n");

    // Test 2: Horizontal win
    clear_board(board);
    board[5][0] = 'O';
    board[5][1] = 'O';
    board[5][2] = 'O';
    board[5][3] = 'O';
    assert(check_winner(board, 'O') == 1);
    assert(check_winner(board, 'X') == 0);
    printf("Test 2 passed: Horizontal win\n");

    // Test 3: Vertical win
    clear_board(board);
    board[5][3] = 'X';
    board[4][3] = 'X';
    board[3][3] = 'X';
    board[2][3] = 'X';
    assert(check_winner(board, 'X') == 1);
    assert(check_winner(board, 'O') == 0);
    printf("Test 3 passed: Vertical win\n");

    // Test 4: Anti-diagonal win
    clear_board(board);
    board[2][6] = 'O';
    board[3][5] = 'O';
    board[4][4] = 'O';
    board[5][3] = 'O';
    assert(check_winner(board, 'O') == 1);
    assert(check_winner(board, 'X') == 0);
    printf("Test 4 passed: Anti-diagonal win\n");

    // Test 5: No winner
    clear_board(board);
    board[5][0] = 'X';
    board[5][1] = 'O';
    board[5][2] = 'X';
    assert(check_winner(board, 'X') == 0);
    assert(check_winner(board, 'O') == 0);
    printf("Test 5 passed: No winner\n");

    // Test 6: Edge case - win at board edges
    clear_board(board);
    board[0][0] = 'X';
    board[0][1] = 'X';
    board[0][2] = 'X';
    board[0][3] = 'X';
    assert(check_winner(board, 'X') == 1);
    printf("Test 6 passed: Win at top edge\n");

    printf("All tests passed!\n");
    return 0;
}