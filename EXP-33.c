#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/des.h>

// Function to handle OpenSSL errors
void handle_openssl_error(void) {
    printf("Error occurred in OpenSSL\n");
    exit(EXIT_FAILURE);
}

// Function to encrypt using DES
void des_encrypt(const unsigned char *plaintext, const unsigned char *key, unsigned char *ciphertext) {
    DES_key_schedule des_key;
    DES_set_key_checked((const_DES_cblock *)key, &des_key);

    DES_ecb_encrypt((const_DES_cblock *)plaintext, (DES_cblock *)ciphertext, &des_key, DES_ENCRYPT);
}

// Function to decrypt using DES
void des_decrypt(const unsigned char *ciphertext, const unsigned char *key, unsigned char *plaintext) {
    DES_key_schedule des_key;
    DES_set_key_checked((const_DES_cblock *)key, &des_key);

    DES_ecb_encrypt((const_DES_cblock *)ciphertext, (DES_cblock *)plaintext, &des_key, DES_DECRYPT);
}

// Function to print a byte array as hex
void print_hex(const unsigned char *array, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02X ", array[i]);
    }
    printf("\n");
}

int main() {
    // 56-bit key (DES key size)
    unsigned char des_key[8] = { 0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xF1 };

    // 64-bit plaintext
    unsigned char plaintext[8] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF };
    unsigned char ciphertext[8];
    unsigned char decrypted[8];

    // Encrypt
    des_encrypt(plaintext, des_key, ciphertext);
    printf("Plaintext: ");
    print_hex(plaintext, 8);
    printf("Ciphertext: ");
    print_hex(ciphertext, 8);

    // Decrypt
    des_decrypt(ciphertext, des_key, decrypted);
    printf("Decrypted: ");
    print_hex(decrypted, 8);

    return 0;
}
OUTPUT:
Plaintext: 01 23 45 67 89 AB CD EF 
Ciphertext: 32 46 E1 01 13 2E 7F 4E 
Decrypted: 01 23 45 67 89 AB CD EF 
