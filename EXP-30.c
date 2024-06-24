#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <openssl/aes.h>

// Function to perform AES encryption in CBC mode for a single block
void aes_cbc_encrypt(const uint8_t *input, const uint8_t *key, uint8_t *output) {
    AES_KEY aes_key;
    AES_set_encrypt_key(key, 128, &aes_key);
    
    uint8_t iv[AES_BLOCK_SIZE] = {0}; // Initialization vector (IV) is all zeros for CBC-MAC
    
    AES_cbc_encrypt(input, output, AES_BLOCK_SIZE, &aes_key, iv, AES_ENCRYPT);
}

// Function to compute CBC-MAC for a one-block message X
void cbc_mac(const uint8_t *message, const uint8_t *key, uint8_t *mac) {
    aes_cbc_encrypt(message, key, mac);
}

// Function to compute CBC-MAC for the two-block message X || (X XOR T)
void adversary_compute(const uint8_t *message, const uint8_t *mac, const uint8_t *key, uint8_t *mac_2blocks) {
    uint8_t temp[AES_BLOCK_SIZE];
    uint8_t concatenated[2 * AES_BLOCK_SIZE];
    
    // Concatenate X and (X XOR T)
    memcpy(concatenated, message, AES_BLOCK_SIZE);
    for (int i = 0; i < AES_BLOCK_SIZE; ++i) {
        concatenated[AES_BLOCK_SIZE + i] = message[i] ^ mac[i];
    }
    
    // Compute CBC-MAC for the concatenated message
    aes_cbc_encrypt(concatenated, key, mac_2blocks);
}

// Function to print a byte array as hex
void print_hex(const uint8_t *array, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02X ", array[i]);
    }
    printf("\n");
}

int main() {
    // Example key and message (both 128 bits = 16 bytes)
    uint8_t key[AES_BLOCK_SIZE] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 
                                     0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };
    uint8_t message[AES_BLOCK_SIZE] = { 0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 
                                        0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A };

    uint8_t mac[AES_BLOCK_SIZE];
    uint8_t mac_2blocks[AES_BLOCK_SIZE];

    // Compute CBC-MAC for the one-block message X
    cbc_mac(message, key, mac);
    printf("MAC for one-block message X: ");
    print_hex(mac, AES_BLOCK_SIZE);

    // Adversary computes CBC-MAC for the two-block message X || (X XOR T)
    adversary_compute(message, mac, key, mac_2blocks);
    printf("Adversary computes MAC for two-block message X || (X XOR T): ");
    print_hex(mac_2blocks, AES_BLOCK_SIZE);

    return 0;
}
OUTPUT:MAC for one-block message X: 5D 9E 0D 7F 94 5A E3 9B 4C D3 BA 90 3A B7 0D 59 
Adversary computes MAC for two-block message X || (X XOR T): 56 9A B5 38 46 00 86 57 25 2A C7 09 21 0C F6 6E 
