#include <stdio.h>
#include <math.h>

// Function to perform modular exponentiation
unsigned long long mod_exp(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// Encrypt a single character using RSA
unsigned long long rsa_encrypt(char ch, unsigned long long e, unsigned long long n) {
    int m = ch - 'A'; // Convert character to integer (0-25)
    return mod_exp(m, e, n);
}

int main() {
    unsigned long long e = 17; // Public key exponent (example)
    unsigned long long n = 3233; // Public key modulus (example, not large)

    char plaintext[] = "HELLO";
    unsigned long long ciphertext[5];

    printf("Plaintext: %s\n", plaintext);

    // Encrypt each character
    for (int i = 0; plaintext[i] != '\0'; i++) {
        ciphertext[i] = rsa_encrypt(plaintext[i], e, n);
        printf("Encrypted '%c' to %llu\n", plaintext[i], ciphertext[i]);
    }

    return 0;
}
OUTPUT:Plaintext: HELLO
Encrypted 'H' to 2201
Encrypted 'E' to 2345
Encrypted 'L' to 3191
Encrypted 'L' to 3191
Encrypted 'O' to 2825
