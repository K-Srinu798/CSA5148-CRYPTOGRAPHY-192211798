#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

// The Playfair key matrix
char playfairMatrix[SIZE][SIZE] = {
    {'M', 'F', 'H', 'I', 'K'},
    {'U', 'N', 'O', 'P', 'Q'},
    {'Z', 'V', 'W', 'X', 'Y'},
    {'E', 'L', 'A', 'R', 'G'},
    {'D', 'S', 'T', 'B', 'C'}
};

// Function to preprocess the message
void preprocessMessage(const char *message, char *preparedMessage) {
    int length = strlen(message);
    int index = 0;

    for (int i = 0; i < length; i++) {
        if (isalpha(message[i])) {
            preparedMessage[index++] = toupper(message[i]);
        }
    }
    preparedMessage[index] = '\0';

    // Replace 'J' with 'I'
    for (int i = 0; i < index; i++) {
        if (preparedMessage[i] == 'J') {
            preparedMessage[i] = 'I';
        }
    }

    // Insert 'X' between repeating characters and at the end if needed
    char temp[200];
    index = 0;
    for (int i = 0; i < strlen(preparedMessage); i++) {
        temp[index++] = preparedMessage[i];
        if (i < strlen(preparedMessage) - 1 && preparedMessage[i] == preparedMessage[i + 1]) {
            temp[index++] = 'X';
        }
    }
    temp[index] = '\0';

    // If the length is odd, add an 'X' at the end
    if (index % 2 != 0) {
        temp[index++] = 'X';
    }
    temp[index] = '\0';

    strcpy(preparedMessage, temp);
}

// Function to find the position of a character in the Playfair matrix
void findPosition(char ch, int *row, int *col) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (playfairMatrix[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

// Function to encrypt a pair of characters using the Playfair matrix
void encryptPair(char ch1, char ch2, char *result) {
    int row1, col1, row2, col2;
    findPosition(ch1, &row1, &col1);
    findPosition(ch2, &row2, &col2);

    if (row1 == row2) {
        // Same row
        result[0] = playfairMatrix[row1][(col1 + 1) % SIZE];
        result[1] = playfairMatrix[row2][(col2 + 1) % SIZE];
    } else if (col1 == col2) {
        // Same column
        result[0] = playfairMatrix[(row1 + 1) % SIZE][col1];
        result[1] = playfairMatrix[(row2 + 1) % SIZE][col2];
    } else {
        // Rectangle
        result[0] = playfairMatrix[row1][col2];
        result[1] = playfairMatrix[row2][col1];
    }
}

// Function to encrypt the message using the Playfair matrix
void encryptMessage(const char *message, char *ciphertext) {
    char preparedMessage[200];
    preprocessMessage(message, preparedMessage);

    int length = strlen(preparedMessage);
    for (int i = 0; i < length; i += 2) {
        encryptPair(preparedMessage[i], preparedMessage[i + 1], &ciphertext[i]);
    }
    ciphertext[length] = '\0';
}

int main() {
    const char message[] = "Must see you over Cadogan West. Coming at once.";
    char ciphertext[200];

    encryptMessage(message, ciphertext);
    printf("Encrypted Message: %s\n", ciphertext);

    return 0;
}
