#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MATRIX_SIZE 3
void getKeyMatrix(char key[], int keyMatrix[MATRIX_SIZE][MATRIX_SIZE]);
void encrypt(char message[], int keyMatrix[MATRIX_SIZE][MATRIX_SIZE], char encryptedMessage[]);
void multiplyMatrices(int keyMatrix[MATRIX_SIZE][MATRIX_SIZE], int messageVector[MATRIX_SIZE], int resultVector[MATRIX_SIZE]);
int main() {
    char key[MATRIX_SIZE * MATRIX_SIZE];
    char message[100];
    char encryptedMessage[100];
    int keyMatrix[MATRIX_SIZE][MATRIX_SIZE];
    printf("Enter the key (9 characters for 3x3 matrix): ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0';  
    printf("Enter the message: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0'; 
    getKeyMatrix(key, keyMatrix);
    encrypt(message, keyMatrix, encryptedMessage);
    printf("Encrypted message: %s\n", encryptedMessage);
    return 0;
}
void getKeyMatrix(char key[], int keyMatrix[MATRIX_SIZE][MATRIX_SIZE]) {
    int i, j, k = 0;

    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            keyMatrix[i][j] = (toupper(key[k]) - 'A') % 26;
            k++;
        }
    }
}
void encrypt(char message[], int keyMatrix[MATRIX_SIZE][MATRIX_SIZE], char encryptedMessage[]) {
    int i, j, messageVector[MATRIX_SIZE], resultVector[MATRIX_SIZE];
    int messageLength = 0;
    for (i = 0; message[i] != '\0'; i++) {
        if (isalpha(message[i])) {
            message[messageLength++] = toupper(message[i]);
        }
    }
    message[messageLength] = '\0';
    while (messageLength % MATRIX_SIZE != 0) {
        message[messageLength++] = 'X';
    }
    message[messageLength] = '\0';
    for (i = 0; i < messageLength; i += MATRIX_SIZE) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            messageVector[j] = message[i + j] - 'A';
        }
        multiplyMatrices(keyMatrix, messageVector, resultVector);
        for (j = 0; j < MATRIX_SIZE; j++) {
            encryptedMessage[i + j] = (resultVector[j] % 26) + 'A';
        }
    }
    encryptedMessage[messageLength] = '\0';
}
void multiplyMatrices(int keyMatrix[MATRIX_SIZE][MATRIX_SIZE], int messageVector[MATRIX_SIZE], int resultVector[MATRIX_SIZE]) {
    int i, j;

    for (i = 0; i < MATRIX_SIZE; i++) {
        resultVector[i] = 0;
        for (j = 0; j < MATRIX_SIZE; j++) {
            resultVector[i] += keyMatrix[i][j] * messageVector[j];
        }
        resultVector[i] = resultVector[i] % 26;
    }
}
