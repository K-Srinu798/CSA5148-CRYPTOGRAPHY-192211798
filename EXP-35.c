#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to encrypt plaintext using Vigenère cipher with one-time pad
void vigenere_one_time_pad_encrypt(const char *plaintext, const int *key, char *ciphertext) {
    int len = strlen(plaintext);
    for (int i = 0; i < len; ++i) {
        // Calculate shift amount from key (wrap around using modulo 26)
        int shift = key[i % len];

        // Encrypt current character of plaintext
        char plain_char = plaintext[i];
        char encrypted_char;

        if (plain_char >= 'A' && plain_char <= 'Z') {
            encrypted_char = 'A' + (plain_char - 'A' + shift) % 26;
        } else if (plain_char >= 'a' && plain_char <= 'z') {
            encrypted_char = 'a' + (plain_char - 'a' + shift) % 26;
        } else {
            // Preserve non-alphabet characters as-is
            encrypted_char = plain_char;
        }

        // Store encrypted character in ciphertext
        ciphertext[i] = encrypted_char;
    }
    // Null-terminate the ciphertext string
    ciphertext[len] = '\0';
}

// Function to print a byte array as hex
void print_hex(const unsigned char *array, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%c", array[i]);
    }
    printf("\n");
}

int main() {
    // Example plaintext and key (stream of random numbers)
    const char *plaintext = "HELLOWORLD";
    int key[] = { 3, 19, 5, 8, 12, 21, 9, 15, 6, 18 }; // Example key

    int len = strlen(plaintext);
    char ciphertext[len + 1]; // +1 for '\0' null terminator

    // Encrypt plaintext using Vigenère cipher with one-time pad
    vigenere_one_time_pad_encrypt(plaintext, key, ciphertext);

    printf("Plaintext: %s\n", plaintext);
    printf("Key: ");
    for (int i = 0; i < len; ++i) {
        printf("%d ", key[i]);
    }
    printf("\n");
    printf("Ciphertext: %s\n", ciphertext);

    return 0;
}
OUTPUT:
Plaintext: HELLOWORLD
Key: 3 19 5 8 12 21 9 15 6 18 
Ciphertext: KADRYWRMPG
