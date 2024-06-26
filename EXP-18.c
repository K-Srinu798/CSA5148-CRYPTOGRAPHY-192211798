#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Initial permutation (IP) table
const int IP[] = {58, 50, 42, 34, 26, 18, 10, 2,
                  60, 52, 44, 36, 28, 20, 12, 4,
                  62, 54, 46, 38, 30, 22, 14, 6,
                  64, 56, 48, 40, 32, 24, 16, 8,
                  57, 49, 41, 33, 25, 17, 9, 1,
                  59, 51, 43, 35, 27, 19, 11, 3,
                  61, 53, 45, 37, 29, 21, 13, 5,
                  63, 55, 47, 39, 31, 23, 15, 7};

// Final permutation (FP) table
const int FP[] = {40, 8, 48, 16, 56, 24, 64, 32,
                  39, 7, 47, 15, 55, 23, 63, 31,
                  38, 6, 46, 14, 54, 22, 62, 30,
                  37, 5, 45, 13, 53, 21, 61, 29,
                  36, 4, 44, 12, 52, 20, 60, 28,
                  35, 3, 43, 11, 51, 19, 59, 27,
                  34, 2, 42, 10, 50, 18, 58, 26,
                  33, 1, 41, 9, 49, 17, 57, 25};

// Expansion table for expanding 32-bit R to 48-bit
const int E[] = {32, 1, 2, 3, 4, 5,
                 4, 5, 6, 7, 8, 9,
                 8, 9, 10, 11, 12, 13,
                 12, 13, 14, 15, 16, 17,
                 16, 17, 18, 19, 20, 21,
                 20, 21, 22, 23, 24, 25,
                 24, 25, 26, 27, 28, 29,
                 28, 29, 30, 31, 32, 1};

// Permutation table after S-box substitution
const int P[] = {16, 7, 20, 21,
                 29, 12, 28, 17,
                 1, 15, 23, 26,
                 5, 18, 31, 10,
                 2, 8, 24, 14,
                 32, 27, 3, 9,
                 19, 13, 30, 6,
                 22, 11, 4, 25};

// Permutation choice 1 table (PC-1)
const int PC1[] = {57, 49, 41, 33, 25, 17, 9,
                   1, 58, 50, 42, 34, 26, 18,
                   10, 2, 59, 51, 43, 35, 27,
                   19, 11, 3, 60, 52, 44, 36,
                   63, 55, 47, 39, 31, 23, 15,
                   7, 62, 54, 46, 38, 30, 22,
                   14, 6, 61, 53, 45, 37, 29,
                   21, 13, 5, 28, 20, 12, 4};

// Permutation choice 2 table (PC-2)
const int PC2[] = {14, 17, 11, 24, 1, 5,
                   3, 28, 15, 6, 21, 10,
                   23, 19, 12, 4, 26, 8,
                   16, 7, 27, 20, 13, 2,
                   41, 52, 31, 37, 47, 55,
                   30, 40, 51, 45, 33, 48,
                   44, 49, 39, 56, 34, 53,
                   46, 42, 50, 36, 29, 32};

// Number of shifts for each round of key schedule
const int SHIFT_COUNT[] = {1, 1, 2, 2, 2, 2, 2, 2,
                           1, 2, 2, 2, 2, 2, 2, 1};

// S-boxes (Substitution boxes)
const uint8_t SBOX[8][4][16] = {
    // S1
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    },
    // S2
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
    },
    // S3
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
    },
    // S4
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
    },
    // S5
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
    },
    // S6
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
    },
    // S7
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
    },
    // S8
    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
    }
};

// Permute using a specified permutation table
void permute(const uint8_t *input, uint8_t *output, const int *table, int table_size) {
    for (int i = 0; i < table_size; ++i) {
        int bit_position = table[i] - 1;
        int byte_position = bit_position / 8;
        int bit_mask = 1 << (7 - (bit_position % 8));
        output[i / 8] |= (input[byte_position] & bit_mask) ? (1 << (7 - (i % 8))) : 0;
    }
}

// Perform circular left shift on a 28-bit key
void circularLeftShift(uint8_t *key, int bits) {
    int bytes = bits / 8;
    int shift = bits % 8;
    uint8_t carry = 0;

    for (int i = 0; i < bytes; ++i) {
        uint8_t temp = key[i];
        key[i] <<= shift;
        key[i] |= carry;
        carry = (temp >> (8 - shift)) & ((1 << shift) - 1);
    }

    if (bytes == 4 && bits == 2) {
        key[3] <<= shift;
        key[3] |= carry;
    }
}

// Generate round keys from the original key
void generateRoundKeys(const uint8_t *key, uint8_t roundKeys[][6]) {
    uint8_t permutedKey[56];
    uint8_t C[4], D[4];

    // Apply permutation choice 1 (PC-1)
    permute(key, permutedKey, PC1, 56);

    // Split permuted key into C and D
    for (int i = 0; i < 28; ++i) {
        C[i / 8] |= (permutedKey[i] << (7 - (i % 8)));
    }
    for (int i = 28; i < 56; ++i) {
        D[(i - 28) / 8] |= (permutedKey[i] << (7 - ((i - 28) % 8)));
    }

    // Generate round keys K1 to K16
    for (int round = 0; round < 16; ++round) {
        // Perform circular left shifts on C and D
        circularLeftShift(C, SHIFT_COUNT[round]);
        circularLeftShift(D, SHIFT_COUNT[round]);

        // Combine C and D back into permutedKey
        for (int i = 0; i < 28; ++i) {
            permutedKey[i] = (C[i / 8] >> (7 - (i % 8))) & 1;
        }
        for (int i = 28; i < 56; ++i) {
            permutedKey[i] = (D[(i - 28) / 8] >> (7 - ((i - 28) % 8))) & 1;
        }

        // Apply permutation choice 2 (PC-2) to generate round key
        for (int i = 0; i < 48; ++i) {
            int bit_position = PC2[i] - 1;
            int byte_position = bit_position / 8;
            int bit_mask = 1 << (7 - (bit_position % 8));
            roundKeys[round][i / 8] |= (permutedKey[bit_position] << (7 - (i % 8)));
        }
    }
}

// XOR two blocks of data
void xorBlocks(const uint8_t *block1, const uint8_t *block2, uint8_t *result, int block_size) {
    for (int i = 0; i < block_size; ++i) {
        result[i] = block1[i] ^ block2[i];
    }
}

// Substitute using S-boxes
void substitute(const uint8_t *input, uint8_t *output) {
    for (int i = 0; i < 8; ++i) {
        int row = ((input[i] & 0x80) >> 4) | ((input[i] & 0x04) >> 2);
        int col = (input[i] & 0x78) >> 3;
        output[i] = SBOX[i][row][col];
    }
}

// Perform Feistel function on 32-bit half-block with 48-bit key
void feistel(const uint8_t *R, const uint8_t *roundKey, uint8_t *output) {
    uint8_t expandedR[6] = {0};
    permute(R, expandedR, E, 48);
    xorBlocks(expandedR, roundKey, output, 6);
    uint8_t substituted[4] = {0};
    substitute(output, substituted);
    permute(substituted, output, P, 32);
}

// Perform DES encryption
void des_decrypt(const uint8_t *cipherText, const uint8_t *key, uint8_t *plainText) {
    uint8_t roundKeys[16][6] = {0};
    generateRoundKeys(key, roundKeys);

    uint8_t block[8] = {0};
    permute(cipherText, block, IP, 64);

    uint8_t L[4] = {0}, R[4] = {0}, temp[4] = {0};
    for (int i = 0; i < 32; ++i) {
        L[i / 8] |= (block[i] << (7 - (i % 8)));
        R[i / 8] |= (block[i + 32] << (7 - (i % 8)));
    }

    for (int round = 15; round >= 0; --round) {
        memcpy(temp, R, 4);
        feistel(R, roundKeys[round], temp);
        xorBlocks(L, temp, temp, 4);
        memcpy(L, R, 4);
        memcpy(R, temp, 4);
    }

    memcpy(block, R, 4);
    memcpy(block + 4, L, 4);
    permute(block, plainText, FP, 64);
}

int main() {
    // Example key and ciphertext for decryption
    uint8_t key[8] = {0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xF1};
    uint8_t ciphertext[8] = {0x0e, 0xd5, 0x8e, 0x4e, 0xb7, 0x59, 0x45, 0x4c};
    uint8_t plaintext[8] = {0};

    // Decrypt ciphertext to plaintext
    des_decrypt(ciphertext, key, plaintext);

    // Print plaintext
    printf("Decrypted plaintext: ");
    for (int i = 0; i < 8; ++i) {
        printf("%02x ", plaintext[i]);
    }
    printf("\n");

    return 0;
}