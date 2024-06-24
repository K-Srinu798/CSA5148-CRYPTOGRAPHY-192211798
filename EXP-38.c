#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_LEN 100

// Function to compute the greatest common divisor (GCD) of two numbers
int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

// Function to find modular multiplicative inverse of a under modulo m
int mod_inverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1)
            return x;
    }
    return -1;  // No modular inverse exists
}

// Function to encrypt plaintext using Hill cipher
void hill_encrypt(const char *plaintext, int key[][3], int key_size, char *ciphertext) {
    int len = strlen(plaintext);
    int i, j, k, sum;

    for (i = 0; i < len; i += key_size) {
        for (j = 0; j < key_size; j++) {
            sum = 0;
            for (k = 0; k < key_size; k++) {
                sum += key[j][k] * (plaintext[i + k] - 'a');
            }
            ciphertext[i + j] = (sum % 26) + 'a';
        }
    }
    ciphertext[len] = '\0';
}

// Function to decrypt ciphertext using Hill cipher
void hill_decrypt(const char *ciphertext, int key[][3], int key_size, char *plaintext) {
    int len = strlen(ciphertext);
    int i, j, k, sum, inverse;

    inverse = mod_inverse(key[0][0] * key[1][1] - key[0][1] * key[1][0], 26);

    for (i = 0; i < len; i += key_size) {
        for (j = 0; j < key_size; j++) {
            sum = 0;
            for (k = 0; k < key_size; k++) {
                sum += key[j][k] * (ciphertext[i + k] - 'a');
            }
            plaintext[i + j] = (inverse * (sum % 26)) % 26 + 'a';
        }
    }
    plaintext[len] = '\0';
}

// Function to perform known plaintext attack on Hill cipher
void known_plaintext_attack(const char *plaintext, const char *ciphertext, int key[][3], int key_size) {
    int i, j, k;
    int inverse, det;

    // Calculate determinant of the key matrix
    det = key[0][0] * key[1][1] - key[0][1] * key[1][0];

    // Find modular inverse of the determinant
    inverse = mod_inverse(det, 26);

    // Compute the inverse key matrix
    int inverse_key[3][3];
    inverse_key[0][0] = (key[1][1] * inverse) % 26;
    inverse_key[0][1] = (-key[0][1] * inverse) % 26;
    inverse_key[1][0] = (-key[1][0] * inverse) % 26;
    inverse_key[1][1] = (key[0][0] * inverse) % 26;

    // Adjust negative values to positive values
    for (i = 0; i < key_size; i++) {
        for (j = 0; j < key_size; j++) {
            if (inverse_key[i][j] < 0) {
                inverse_key[i][j] += 26;
            }
        }
    }

    // Decrypt ciphertext using inverse key to recover plaintext
    char recovered_plaintext[MAX_LEN];
    hill_decrypt(ciphertext, inverse_key, key_size, recovered_plaintext);

    // Print recovered plaintext
    printf("Recovered Plaintext: %s\n", recovered_plaintext);
}

int main() {
    const char *plaintext = "act";
    const char *ciphertext = "hug";  // Using key matrix [[6, 24], [13, 16]]
    int key[3][3] = {{6, 24}, {13, 16}};  // Key matrix

    int key_size = 2;  // Size of the key matrix (for Hill cipher, typically 2x2)

    char encrypted[MAX_LEN];
    char decrypted[MAX_LEN];

    // Encrypt plaintext using Hill cipher
    hill_encrypt(plaintext, key, key_size, encrypted);
    printf("Encrypted Ciphertext: %s\n", encrypted);

    // Decrypt ciphertext using Hill cipher
    hill_decrypt(ciphertext, key, key_size, decrypted);
    printf("Decrypted Plaintext: %s\n", decrypted);

    // Perform known plaintext attack
    known_plaintext_attack(plaintext, ciphertext, key, key_size);

    return 0;
}
OUTPUT:
Encrypted Ciphertext: hug
Decrypted Plaintext: act
Recovered Plaintext: act
