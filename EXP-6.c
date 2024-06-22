#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to find the greatest common divisor (GCD)
int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Function to find the modular multiplicative inverse of a under modulo 26
int modInverse(int a) {
    for (int x = 1; x < 26; x++) {
        if ((a * x) % 26 == 1) {
            return x;
        }
    }
    return -1;
}

// Function to decrypt the ciphertext using the affine cipher
void decrypt(char ciphertext[], int a, int b, char plaintext[]) {
    int a_inv = modInverse(a);
    if (a_inv == -1) {
        printf("Error: a does not have a modular inverse under modulo 26.\n");
        return;
    }

    int textLen = strlen(ciphertext);
    for (int i = 0; i < textLen; i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            plaintext[i] = (a_inv * ((ciphertext[i] - base) - b + 26)) % 26 + base;
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[textLen] = '\0';
}

// Function to solve for a and b given the ciphertext mappings
void solveAffineCipher() {
    int a, b;
    int x1 = 1, y1 = 4; // 'B' -> 'E'
    int x2 = 20, y2 = 19; // 'U' -> 'T'

    // Solve for a
    int a_candidate = (y2 - y1 + 26) % 26;
    int a_mod = (x2 - x1 + 26) % 26;
    int a_inv_mod = modInverse(a_mod);
    a = (a_candidate * a_inv_mod) % 26;

    // Solve for b
    b = (y1 - a * x1 + 26) % 26;

    printf("Found values: a = %d, b = %d\n", a, b);

    // Example ciphertext
    char ciphertext[] = "BUV";
    char plaintext[100];

    decrypt(ciphertext, a, b, plaintext);
    printf("Decrypted text: %s\n", plaintext);
}

int main() {
    solveAffineCipher();
    return 0;
}
