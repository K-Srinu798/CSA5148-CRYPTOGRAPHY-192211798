#include <stdio.h>
#include <math.h>

// Function to calculate factorial
unsigned long long factorial(int n) {
    unsigned long long result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main() {
    int n = 25;
    unsigned long long fact = factorial(n);
    double log2_fact = log2((double)fact);

    printf("Number of possible keys for Playfair cipher: %llu\n", fact);
    printf("Approximate number of possible keys as a power of 2: 2^%.2f\n", log2_fact);

    return 0;
}
