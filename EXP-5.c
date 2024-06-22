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

// Function to check if a is coprime with 26
int isCoprime(int a) {
    return gcd(a, 26) == 1;
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

// Function to encrypt the plaintext using the affine Caesar cipher
void encrypt(char plaintext[], int a, int b, char ciphertext[]) {
    if (!isCoprime(a)) {
        printf("Error: a is not coprime with 26.\n");
        exit(1);
    }

    int textLen = strlen(plaintext);
    for (int i = 0; i < textLen; i++) {
        if (isalpha(plaintext[i])) {
            char base = isupper(plaintext[i]) ? 'A' : 'a';
            ciphertext[i] = ((a * (plaintext[i] - base) + b) % 26) + base;
        } else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[textLen] = '\0';
}

// Function to decrypt the ciphertext using the affine Caesar cipher
void decrypt(char ciphertext[], int a, int b, char plaintext[]) {
    if (!isCoprime(a)) {
        printf("Error: a is not coprime with 26.\n");
        exit(1);
    }

    int a_inv = modInverse(a);
    int textLen = strlen(ciphertext);
    for (int i = 0; i < textLen; i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            plaintext[i] = (a_inv * ((ciphertext[i] - base) - b + 26) % 26) + base;
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[textLen] = '\0';
}

int main() {
    char plaintext[100], ciphertext[100];
    int a, b;

    printf("Enter the plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0';  // Remove newline character

    printf("Enter the value of a: ");
    scanf("%d", &a);
    printf("Enter the value of b: ");
    scanf("%d", &b);

    // Ensure b is in the valid range
    b = b % 26;

    encrypt(plaintext, a, b, ciphertext);
    printf("Encrypted text: %s\n", ciphertext);

    char decryptedtext[100];
    decrypt(ciphertext, a, b, decryptedtext);
    printf("Decrypted text: %s\n", decryptedtext);

    return 0;
}
