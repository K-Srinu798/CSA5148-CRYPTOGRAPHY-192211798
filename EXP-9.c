#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

// Function to remove duplicate characters from a keyword
void removeDuplicates(char *str) {
    int index = 0;
    int n = strlen(str);

    for (int i = 0; i < n; i++) {
        int j;
        for (j = 0; j < i; j++) {
            if (str[i] == str[j]) {
                break;
            }
        }
        if (j == i) {
            str[index++] = str[i];
        }
    }
    str[index] = '\0';
}

// Function to generate the Playfair key square
void generateKeySquare(char keySquare[SIZE][SIZE], char *keyword) {
    char alphabet[] = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
    int alphabetUsed[26] = {0};
    int index = 0;

    // Remove duplicates from keyword and replace 'J' with 'I'
    removeDuplicates(keyword);
    for (int i = 0; keyword[i] != '\0'; i++) {
        if (keyword[i] == 'J') {
            keyword[i] = 'I';
        }
        keySquare[index / SIZE][index % SIZE] = keyword[i];
        alphabetUsed[keyword[i] - 'A'] = 1;
        index++;
    }

    // Fill the remaining key square with other letters
    for (int i = 0; i < 25; i++) {
        if (!alphabetUsed[alphabet[i] - 'A']) {
            keySquare[index / SIZE][index % SIZE] = alphabet[i];
            index++;
        }
    }
}

// Function to find the position of a character in the key square
void findPosition(char keySquare[SIZE][SIZE], char ch, int *row, int *col) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (keySquare[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

// Function to decrypt a pair of characters using the Playfair key square
void decryptPair(char keySquare[SIZE][SIZE], char ch1, char ch2, char *result) {
    int row1, col1, row2, col2;
    findPosition(keySquare, ch1, &row1, &col1);
    findPosition(keySquare, ch2, &row2, &col2);

    if (row1 == row2) {
        // Same row
        result[0] = keySquare[row1][(col1 + SIZE - 1) % SIZE];
        result[1] = keySquare[row2][(col2 + SIZE - 1) % SIZE];
    } else if (col1 == col2) {
        // Same column
        result[0] = keySquare[(row1 + SIZE - 1) % SIZE][col1];
        result[1] = keySquare[(row2 + SIZE - 1) % SIZE][col2];
    } else {
        // Rectangle
        result[0] = keySquare[row1][col2];
        result[1] = keySquare[row2][col1];
    }
}

// Function to decrypt the ciphertext using the Playfair key square
void decryptMessage(char keySquare[SIZE][SIZE], char *ciphertext, char *plaintext) {
    int length = strlen(ciphertext);
    for (int i = 0; i < length; i += 2) {
        decryptPair(keySquare, toupper(ciphertext[i]), toupper(ciphertext[i + 1]), &plaintext[i]);
    }
    plaintext[length] = '\0';
}

int main() {
    char keyword[] = "KEYWORD";
    char keySquare[SIZE][SIZE];
    generateKeySquare(keySquare, keyword);

    printf("Playfair Key Square:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", keySquare[i][j]);
        }
        printf("\n");
    }

    char ciphertext[] = "KXJEYUREBEZWEHEWRYTUHEYFSKREHEGOYFIWTTTUOLKSYCAJPOBOTEIZONTXBYBNTGONEYCZWRGDSONSXBOUYWRHEBAAHYUSEDQ";
    char plaintext[200];

    decryptMessage(keySquare, ciphertext, plaintext);
    printf("Decrypted Message: %s\n", plaintext);

    return 0;
}
