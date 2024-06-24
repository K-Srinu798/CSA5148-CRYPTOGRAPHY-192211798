#include <stdio.h>
#include <stdlib.h>
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

int main() {
    unsigned long long q = 23; // Example prime number
    unsigned long long a = 5;  // Example primitive root

    // Alice's secret number
    unsigned long long xA = 6;
    // Bob's secret number
    unsigned long long xB = 15;

    // Alice computes her public value
    unsigned long long A = mod_exp(a, xA, q);
    // Bob computes his public value
    unsigned long long B = mod_exp(a, xB, q);

    // Exchange public values (A and B)

    // Alice computes the shared secret
    unsigned long long shared_secret_A = mod_exp(B, xA, q);
    // Bob computes the shared secret
    unsigned long long shared_secret_B = mod_exp(A, xB, q);

    // Both should be equal
    printf("Alice's Shared Secret: %llu\n", shared_secret_A);
    printf("Bob's Shared Secret: %llu\n", shared_secret_B);

    return 0;
}
OUTPUT:Alice's Shared Secret: 2
Bob's Shared Secret: 2
