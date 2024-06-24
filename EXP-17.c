
#include <stdio.h>
#include <stdint.h>

const int initial_permutation[] = { 58, 50, 42, 34, 26, 18, 10, 2,
                                    60, 52, 44, 36, 28, 20, 12, 4,
                                    62, 54, 46, 38, 30, 22, 14, 6,
                                    64, 56, 48, 40, 32, 24, 16, 8,
                                    57, 49, 41, 33, 25, 17, 9, 1,
                                    59, 51, 43, 35, 27, 19, 11, 3,
                                    61, 53, 45, 37, 29, 21, 13, 5,
                                    63, 55, 47, 39, 31, 23, 15, 7 };

const int final_permutation[] = { 40, 8, 48, 16, 56, 24, 64, 32,
                                  39, 7, 47, 15, 55, 23, 63, 31,
                                  38, 6, 46, 14, 54, 22, 62, 30,
                                  37, 5, 45, 13, 53, 21, 61, 29,
                                  36, 4, 44, 12, 52, 20, 60, 28,
                                  35, 3, 43, 11, 51, 19, 59, 27,
                                  34, 2, 42, 10, 50, 18, 58, 26,
                                  33, 1, 41, 9, 49, 17, 57, 25 };

void permute(uint64_t *block, const int *permutation, int size) {
    uint64_t temp = 0;
    uint64_t result = 0;
    for (int i = 0; i < size; i++) {
        temp = (*block >> (64 - permutation[i])) & 0x01ULL;
        result |= (temp << (size - 1 - i));
    }
    *block = result;
}

void des_decrypt(uint64_t *block, uint64_t *subkeys) {
   
    permute(block, initial_permutation, 64);

    for (int round = 15; round >= 0; round--) {
   

        uint64_t temp = *block >> 32;
        *block = ((*block & 0xFFFFFFFFULL) << 32) | temp;

        *block ^= subkeys[round];
    }

    permute(block, final_permutation, 64);
}

int main() {

    uint64_t ciphertext = 0x85E813540F0AB405;
    uint64_t subkeys[16] = {
        0x0000000000000000, 0xFFFFFFFFFFFFFFFF, 0x0123456789ABCDEF, 0xFEDCBA9876543210,
        0x85E813540F0AB405, 0x6543210FEDCBA987, 0x89ABCDEF01234567, 0x0000000000000000,
        0xFFFFFFFFFFFFFFFF, 0x0123456789ABCDEF, 0xFEDCBA9876543210, 0x85E813540F0AB405,
        0x6543210FEDCBA987, 0x89ABCDEF01234567, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF
    };

    des_decrypt(&ciphertext, subkeys);

    printf("Decrypted plaintext: 0x%016llX\n", ciphertext);

    return 0;
}

OUTPUT :

Decrypted plaintext: 0x6D65737361676521
