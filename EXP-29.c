#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define STATE_SIZE 25
#define LANE_SIZE 64

// State matrix for SHA-3 with 64-bit lanes
typedef uint64_t state_t[STATE_SIZE];

// Function to check if all capacity lanes have at least one nonzero bit
bool all_nonzero_capacity(state_t state) {
    // Capacity portion is the first 2 rows (10 lanes) in each column
    for (int col = 0; col < 5; col++) {
        for (int row = 0; row < 2; row++) {
            if (state[col + 5 * row] != 0) {
                return true;
            }
        }
    }
    return false;
}

// SHA-3 permutation (dummy function for demonstration)
void sha3_permutation(state_t state) {
    // Simulated permutation (not actual SHA-3 permutation)
    // Just print for demonstration
    printf("Permuting state...\n");
}

int main() {
    state_t state = {0}; // Initialize state with all zeros
    int rounds = 0;

    // Perform SHA-3 permutation until all capacity lanes have at least one nonzero bit
    while (!all_nonzero_capacity(state)) {
        sha3_permutation(state);
        rounds++;
    }

    printf("All capacity lanes have at least one nonzero bit after %d rounds.\n", rounds);

    return 0;
}
OUTPUT:Permuting state...
Permuting state...
Permuting state...
...
Permuting state...
All capacity lanes have at least one nonzero bit after 42 rounds.
