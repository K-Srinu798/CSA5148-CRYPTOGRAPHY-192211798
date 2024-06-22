#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function to calculate the modular inverse
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1)
            return x;
    }
    return -1;
}

// Function to calculate the determinant of a 2x2 matrix
int determinant(int matrix[2][2], int mod) {
    int det = (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]) % mod;
    if (det < 0) det += mod;
    return det;
}

// Function to find the adjugate of a 2x2 matrix
void adjugate(int matrix[2][2], int adj[2][2]) {
    adj[0][0] =  matrix[1][1];
    adj[0][1] = -matrix[0][1];
    adj[1][0] = -matrix[1][0];
    adj[1][1] =  matrix[0][0];
}

// Function to find the inverse of a 2x2 matrix mod 26
void inverseMatrix(int matrix[2][2], int inv[2][2], int mod) {
    int det = determinant(matrix, mod);
    int det_inv = modInverse(det, mod);
    if (det_inv == -1) {
        printf("Matrix is not invertible\n");
        exit(1);
    }

    int adj[2][2];
    adjugate(matrix, adj);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            inv[i][j] = (adj[i][j] * det_inv) % mod;
            if (inv[i][j] < 0) inv[i][j] += mod;
        }
    }
}

// Function to multiply two 2x2 matrices
void multiplyMatrices(int a[2][2], int b[2][2], int res[2][2], int mod) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            res[i][j] = 0;
            for (int k = 0; k < 2; k++) {
                res[i][j] += a[i][k] * b[k][j];
            }
            res[i][j] %= mod;
        }
    }
}

// Function to solve for the key matrix given plaintext-ciphertext pairs
void solveKey(int plaintext[2][2], int ciphertext[2][2], int key[2][2], int mod) {
    int plaintext_inv[2][2];
    inverseMatrix(plaintext, plaintext_inv, mod);

    multiplyMatrices(ciphertext, plaintext_inv, key, mod);
}

int main() {
    // Plaintext-ciphertext pairs
    int plaintext[2][2] = {{19, 7}, {4, 11}}; // Represents "TH", "EL"
    int ciphertext[2][2] = {{6, 17}, {24, 21}}; // Corresponding ciphertext

    int key[2][2];

    // Solve for the key matrix
    solveKey(plaintext, ciphertext, key, 26);

    // Print the key matrix
    printf("Key matrix:\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            printf("%d ", key[i][j]);
        }
        printf("\n");
    }

    return 0;
}
