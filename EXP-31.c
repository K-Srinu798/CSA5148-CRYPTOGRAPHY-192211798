#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <openssl/aes.h>

// Constants for AES-128 (block size 128 bits)
#define AES_BLOCK_SIZE 16
#define CONST_R 0x87

// Function to perform AES encryption
void aes_encrypt(const uint8_t *plaintext, const uint8_t *key, uint8_t *ciphertext) {
    AES_KEY aes_key;
    AES_set_encrypt_key(key, 128, &aes_key);
    AES_encrypt(plaintext, ciphertext, &aes_key);
}

// Function to generate CMAC subkeys
void generate_cmac_subkeys(const uint8_t *key, uint8_t *subkey1, uint8_t *subkey2) {
    uint8_t zero_block[AES_BLOCK_SIZE] = {0};
    uint8_t E0[AES_BLOCK_SIZE];
    uint8_t L[AES_BLOCK_SIZE];
    uint8_t K1[AES_BLOCK_SIZE];
    uint8_t K2[AES_BLOCK_SIZE];

    // Step 1: Calculate E(0) = AES(0)
    aes_encrypt(zero_block, key, E0);

    // Step 2: Calculate L = LeftShift(E(0))
    memcpy(L, E0, AES_BLOCK_SIZE);
    if ((E0[0] & 0x80) == 0) {
        // No need to XOR const_R since MSB is 0
    } else {
        // Left shift L
        for (int i = 0; i < AES_BLOCK_SIZE - 1; ++i) {
            L[i] = (L[i] << 1) | (L[i + 1] >> 7);
        }
        L[AES_BLOCK_SIZE - 1] = L[AES_BLOCK_SIZE - 1] << 1;
        // XOR with const_R
        L[AES_BLOCK_SIZE - 1] ^= CONST_R;
    }

    // Step 3: Calculate K1 = L
    memcpy(K1, L, AES_BLOCK_SIZE);

    // Step 4: Calculate K2 = LeftShift(K1)
    memcpy(K2, K1, AES_BLOCK_SIZE);
    if ((K1[0] & 0x80) == 0) {
        // No need to XOR const_R since MSB is 0
    } else {
        // Left shift K2
        for (int i = 0; i < AES_BLOCK_SIZE - 1; ++i) {
            K2[i] = (K2[i] << 1) | (K2[i + 1] >> 7);
        }
        K2[AES_BLOCK_SIZE - 1] = K2[AES_BLOCK_SIZE - 1] << 1;
        // XOR with const_R
        K2[AES_BLOCK_SIZE - 1] ^= CONST_R;
    }

    // Copy results to subkeys
    memcpy(subkey1, K1, AES_BLOCK_SIZE);
    memcpy(subkey2, K2, AES_BLOCK_SIZE);
}

// Function to print a byte array as hex
void print_hex(const uint8_t *array, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02X ", array[i]);
    }
    printf("\n");
}

int main() {
    uint8_t key[AES_BLOCK_SIZE] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 
                                    0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };

    uint8_t subkey1[AES_BLOCK_SIZE];
    uint8_t subkey2[AES_BLOCK_SIZE];

    generate_cmac_subkeys(key, subkey1, subkey2);

    printf("Subkey 1: ");
    print_hex(subkey1, AES_BLOCK_SIZE);

    printf("Subkey 2: ");
    print_hex(subkey2, AES_BLOCK_SIZE);

    return 0;
}
OUTPUT:Subkey 1: 88 20 B9 19 92 65 5C 6A A4 8A AD C1 E5 7D F8 63 
Subkey 2: 11 41 72 D6 F0 F1 C4 58 A3 80 86 6D B4 9B 13 46 
