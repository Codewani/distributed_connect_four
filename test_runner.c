#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // Execute a command to list files in the current directory (Unix-like systems)
    system("cd tests && gcc -o test_check_winner test_check_winner.c && ./test_check_winner && gcc -o test_hash_map test_hash_map.c && ./test_hash_map && rm test_check_winner && rm test_hash_map && cd ..");
    // Execute a command to list files in the current directory (Windows)
    // system("dir"); 

    // Execute a command to create a file
    system("echo Hello, World! > output.txt"); 

    // Check if the command processor is available
    if (system(NULL) != 0) {
        // Command processor is available
    } else {
        // Command processor is not available
    }
}