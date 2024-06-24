#include <stdio.h>

// Function to calculate the greatest common divisor (GCD) using Euclid's algorithm
int gcd(int a, int b) {
    int temp;
    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to find the multiplicative inverse of a modulo m using extended Euclidean algorithm
int mod_inverse(int a, int m) {
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;

    if (m == 1) return 0;

    while (a > 1) {
        // q is quotient
        q = a / m;
        t = m;

        // m is remainder now, process same as Euclid's algo
        m = a % m, a = t;

        t = x0;

        // Update x and y
        x0 = x1 - q * x0;

        x1 = t;
    }
    
    return x1 < 0 ? x1 + m0 : x1;
}

// Function to encrypt plaintext using affine Caesar cipher
void affine_caesar_encrypt(const char *plaintext, int a, int b, char *ciphertext) {
    int i = 0;
    while (plaintext[i] != '\0') {
        if (plaintext[i] >= 'A' && plaintext[i] <= 'Z') {
            int p = plaintext[i] - 'A';
            int C = (a * p + b) % 26;
            ciphertext[i] = 'A' + C;
        } else if (plaintext[i] >= 'a' && plaintext[i] <= 'z') {
            int p = plaintext[i] - 'a';
            int C = (a * p + b) % 26;
            ciphertext[i] = 'a' + C;
        } else {
            // Preserve non-alphabet characters as-is
            ciphertext[i] = plaintext[i];
        }
        i++;
    }
    ciphertext[i] = '\0';
}

// Function to decrypt ciphertext using affine Caesar cipher
void affine_caesar_decrypt(const char *ciphertext, int a, int b, char *plaintext) {
    // Find multiplicative inverse of 'a' modulo 26
    int a_inverse = mod_inverse(a, 26);

    int i = 0;
    while (ciphertext[i] != '\0') {
        if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
            int C = ciphertext[i] - 'A';
            int p = (a_inverse * (C - b + 26)) % 26;
            plaintext[i] = 'A' + p;
        } else if (ciphertext[i] >= 'a' && ciphertext[i] <= 'z') {
            int C = ciphertext[i] - 'a';
            int p = (a_inverse * (C - b + 26)) % 26;
            plaintext[i] = 'a' + p;
        } else {
            // Preserve non-alphabet characters as-is
            plaintext[i] = ciphertext[i];
        }
        i++;
    }
    plaintext[i] = '\0';
}

// Function to print a string
void print_string(const char *str) {
    printf("%s\n", str);
}

int main() {
    // Example usage
    const char *plaintext = "HELLO";
    int a = 3;  // Multiplicative coefficient (must be coprime with 26)
    int b = 7;  // Shift amount

    char ciphertext[100];
    char decrypted[100];

    // Encrypt plaintext
    affine_caesar_encrypt(plaintext, a, b, ciphertext);

    printf("Plaintext: ");
    print_string(plaintext);
    printf("Ciphertext (a=%d, b=%d): ", a, b);
    print_string(ciphertext);

    // Decrypt ciphertext
    affine_caesar_decrypt(ciphertext, a, b, decrypted);

    printf("Decrypted: ");
    print_string(decrypted);

    return 0;
}
OUTPUT:
Plaintext: HELLO
Ciphertext (a=3, b=7): AXEEH
Decrypted: HELLO
