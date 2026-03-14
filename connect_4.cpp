#include <unordered_map>
#include <utility>
#include <functional>
#include "connect_4_helpers.h"

using namespace std;

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;  // simple hash combination
    }
};

unordered_map<pair<int, int>, int, hash_pair> bottom;
unordered_map<pair<int, int>, int, hash_pair> left_dir;
unordered_map<pair<int, int>, int, hash_pair> top_left;
unordered_map<pair<int, int>, int, hash_pair> top_right;
unordered_map<pair<int, int>, int, hash_pair> bottom_left;
unordered_map<pair<int, int>, int, hash_pair> right_dir;
unordered_map<pair<int, int>, int, hash_pair> bottom_right;
unordered_map<pair<int, int>, int, hash_pair>* dir_dictionaries[7];
unordered_map<pair<int, int>, int, hash_pair> valid_spaces;
int directions[7][2] = {{-1, 0}, {0, -1}, {1, -1}, {1, 1}, {-1, -1}, {0, 1}, {-1, 1}};

void initialize(void) {
    dir_dictionaries[0] = &bottom;
    dir_dictionaries[1] = &left_dir;
    dir_dictionaries[2] = &top_left;
    dir_dictionaries[3] = &top_right;
    dir_dictionaries[4] = &bottom_left;
    dir_dictionaries[5] = &right_dir;
    dir_dictionaries[6] = &bottom_right;
    
    for (int j = 0; j < 7; j++) {
        pair<int, int> valid_pos = {5, j};
        valid_spaces[valid_pos] = 0;
    }
}

int is_valid_move(int i, int j) {
    pair<int, int> coord = {i, j};
    return valid_spaces.count(coord) > 0;
}

void make_move(int i, int j, char player) {
    pair<int, int> coord = {i, j};
    valid_spaces.erase(coord);
    if (i > 0) {
        pair<int, int> new_valid = {i - 1, j};
        valid_spaces[new_valid] = 0;
    }
}

int check_winner(char board[6][7], char player) {
    for (int i = 5; i >= 0; i--) {
        for (int j = 0; j < 7; j++) {
            pair<int, int> coord = {i, j};
            if (board[i][j] == player) {
                for (int k = 0; k < 7; k++){
                    pair<int, int> prev = {i + directions[k][0], j + directions[k][1]};
                    if (dir_dictionaries[k]->count(prev) == 1){
                        int count = (*dir_dictionaries[k])[prev];
                        if (count == 3) {
                            return 1;
                        }
                        else {
                            (*dir_dictionaries[k])[coord] = count + 1;
                        }
                    }
                    else {
                        (*dir_dictionaries[k])[coord] = 1;
                    }
                }
            }
        } 
    }
    return 0;
}