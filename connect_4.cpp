#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <functional>
#include <tuple>
#include "connect_4_helpers.h"
#include <iostream>

using namespace std;

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;  // simple hash combination
    }
};

struct hash_tuple {
    template <class T1, class T2, class T3>
    size_t operator()(const tuple<T1, T2, T3>& t) const {
        auto h1 = hash<T1>{}(get<0>(t));
        auto h2 = hash<T2>{}(get<1>(t));
        auto h3 = hash<T3>{}(get<2>(t));
        return h1 ^ h2 ^ h3;
    }
};

unordered_map<tuple<int, int, char>, int, hash_tuple> bottom;
unordered_map<tuple<int, int, char>, int, hash_tuple> left_dir;
unordered_map<tuple<int, int, char>, int, hash_tuple> top_left;
unordered_map<tuple<int, int, char>, int, hash_tuple> top_right;
unordered_map<tuple<int, int, char>, int, hash_tuple> bottom_left;
unordered_map<tuple<int, int, char>, int, hash_tuple> right_dir;
unordered_map<tuple<int, int, char>, int, hash_tuple> bottom_right;
unordered_map<tuple<int, int, char>, int, hash_tuple>* dir_dictionaries[7];
unordered_set<pair<int, int>, hash_pair> valid_spaces;
int directions[7][2] = {{1, 0}, {0, -1}, {1, -1}, {1, 1}, {-1, -1}, {0, 1}, {-1, 1}};

void initialize(void) {
    dir_dictionaries[0] = &bottom;
    dir_dictionaries[1] = &left_dir;
    dir_dictionaries[2] = &top_left;
    dir_dictionaries[3] = &top_right;
    dir_dictionaries[4] = &bottom_left;
    dir_dictionaries[5] = &right_dir;
    dir_dictionaries[6] = &bottom_right;
    
    for (int j = 0; j < 7; j++) {
        valid_spaces.insert({5, j});
    }
}

int is_valid_move(int i, int j) {
    pair<int, int> coord = {i, j};
    return valid_spaces.find(coord) != valid_spaces.end();
}

int make_move(int i, int j, char player) {
    pair<int, int> coord = {i, j};
    valid_spaces.erase(coord);
    if (i > 0) {
        pair<int, int> new_valid = {i - 1, j};
        valid_spaces.insert(new_valid);
    }
    
    // Update counts for each direction
    for (int k = 0; k < 7; k++){
        tuple<int, int, char> current = {i, j, player};
        tuple<int, int, char> prev = {i + directions[k][0], j + directions[k][1], player};
        auto& dict = *dir_dictionaries[k];
        
        int count = 1;
        if (dict.find(prev) != dict.end()){
            count += dict[prev];
        }
        dict[current] = count;
    }
    
    // Check for wins by combining opposite directions
    // Horizontal: left + right
    int horizontal = (*dir_dictionaries[1])[{i, j, player}] + (*dir_dictionaries[5])[{i, j, player}] - 1;
    // Vertical: bottom only (pieces fall down)
    int vertical = (*dir_dictionaries[0])[{i, j, player}];
    // Diagonal 1: top-left + bottom-right
    int diag1 = (*dir_dictionaries[2])[{i, j, player}] + (*dir_dictionaries[6])[{i, j, player}] - 1;
    // Diagonal 2: top-right + bottom-left
    int diag2 = (*dir_dictionaries[3])[{i, j, player}] + (*dir_dictionaries[4])[{i, j, player}] - 1;
    
    if (horizontal >= 4 || vertical >= 4 || diag1 >= 4 || diag2 >= 4) {
        return 1;
    }
    return 0;
}
