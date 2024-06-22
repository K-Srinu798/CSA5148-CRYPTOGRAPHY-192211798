#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 2

void encrypt(int key[SIZE][SIZE], char *plaintext, char *ciphertext) {
    int length = strlen(plaintext);
    int padded_length = length + (length % SIZE);
    int p[SIZE], c[SIZE];
    
    for (int i = 0; i < padded_length; i += SIZE) {
        for (int j = 0; j < SIZE; j++) {
            p[j] = (i + j < length) ? (toupper(plaintext[i + j]) - 'A') : ('X' - 'A');
        }

        c[0] = (key[0][0] * p[0] + key[0][1] * p[1]) % 26;
        c[1] = (key[1][0] * p[0] + key[1][1] * p[1]) % 26;

        for (int j = 0; j < SIZE; j++) {
            ciphertext[i + j] = c[j] + 'A';
        }
    }
    ciphertext[padded_length] = '\0';
}

void decrypt(int key_inv[SIZE][SIZE], char *ciphertext, char *plaintext) {
    int length = strlen(ciphertext);
    int p[SIZE], c[SIZE];
    
    for (int i = 0; i < length; i += SIZE) {
        for (int j = 0; j < SIZE; j++) {
            c[j] = toupper(ciphertext[i + j]) - 'A';
        }

        p[0] = (key_inv[0][0] * c[0] + key_inv[0][1] * c[1]) % 26;
        p[1] = (key_inv[1][0] * c[0] + key_inv[1][1] * c[1]) % 26;

        for (int j = 0; j < SIZE; j++) {
            plaintext[i + j] = p[j] + 'A';
        }
    }
    plaintext[length] = '\0';
}

int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1;
}

void findKeyInverse(int key[SIZE][SIZE], int key_inv[SIZE][SIZE], int m) {
    int det = (key[0][0] * key[1][1] - key[0][1] * key[1][0]) % m;
    if (det < 0) det += m;

    int det_inv = modInverse(det, m);
    if (det_inv == -1) {
        printf("Inverse doesn't exist\n");
        exit(1);
    }

    key_inv[0][0] = ( key[1][1] * det_inv) % m;
    key_inv[1][1] = ( key[0][0] * det_inv) % m;
    key_inv[0][1] = (-key[0][1] * det_inv) % m;
    key_inv[1][0] = (-key[1][0] * det_inv) % m;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (key_inv[i][j] < 0) {
                key_inv[i][j] += m;
            }
        }
    }
}

int main() {
    char plaintext[] = "meet me at the usual place at ten rather than eight oclock";
    char ciphertext[200], decryptedtext[200];
    int key[SIZE][SIZE] = {{9, 4}, {5, 7}};
    int key_inv[SIZE][SIZE];

    // Find the inverse key matrix
    findKeyInverse(key, key_inv, 26);

    // Encrypt the plaintext
    encrypt(key, plaintext, ciphertext);
    printf("Encrypted Message: %s\n", ciphertext);

    // Decrypt the ciphertext
    decrypt(key_inv, ciphertext, decryptedtext);
    printf("Decrypted Message: %s\n", decryptedtext);

    return 0;
}
