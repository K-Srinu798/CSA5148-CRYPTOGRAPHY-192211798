#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5
void generateKeyTable(char key[], char keyTable[SIZE][SIZE]);
void search(char keyTable[SIZE][SIZE], char a, char b, int *row1, int *col1, int *row2, int *col2);
int mod5(int a);
void encrypt(char str[], char keyTable[SIZE][SIZE]);
int main() {
    char key[SIZE * SIZE], str[100];
    char keyTable[SIZE][SIZE];
    printf("Enter the key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0'; 
    printf("Enter the plaintext: ");
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = '\0'; 

    generateKeyTable(key, keyTable);
    encrypt(str, keyTable);

    printf("Encrypted text: %s\n", str);

    return 0;
}
void generateKeyTable(char key[], char keyTable[SIZE][SIZE]) {
    int i, j, k, index = 0;
    int used[26] = {0};
    used['J' - 'A'] = 1;
    for (i = 0; key[i] != '\0'; i++) {
        key[i] = toupper(key[i]);
        if (!used[key[i] - 'A']) {
            used[key[i] - 'A'] = 1;
            keyTable[index / SIZE][index % SIZE] = key[i];
            index++;
        }
    }
    for (k = 0; k < 26; k++) {
        if (!used[k]) {
            keyTable[index / SIZE][index % SIZE] = 'A' + k;
            index++;
        }
    }
}
void search(char keyTable[SIZE][SIZE], char a, char b, int *row1, int *col1, int *row2, int *col2) {
    int i, j;

    if (a == 'J') a = 'I';
    if (b == 'J') b = 'I';

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (keyTable[i][j] == a) {
                *row1 = i;
                *col1 = j;
            }
            if (keyTable[i][j] == b) {
                *row2 = i;
                *col2 = j;
            }
        }
    }
}
int mod5(int a) {
    return (a % SIZE);
}
void encrypt(char str[], char keyTable[SIZE][SIZE]) {
    int i, len, row1, col1, row2, col2;
    for (i = 0, len = 0; str[i] != '\0'; i++) {
        if (str[i] != ' ') {
            str[len++] = toupper(str[i]);
        }
    }
    str[len] = '\0';
    if (len % 2 != 0) {
        str[len++] = 'X';
        str[len] = '\0';
    }
    for (i = 0; i < len; i += 2) {
        search(keyTable, str[i], str[i + 1], &row1, &col1, &row2, &col2);

        if (row1 == row2) {
            str[i] = keyTable[row1][mod5(col1 + 1)];
            str[i + 1] = keyTable[row2][mod5(col2 + 1)];
        } else if (col1 == col2) {
            str[i] = keyTable[mod5(row1 + 1)][col1];
            str[i + 1] = keyTable[mod5(row2 + 1)][col2];
        } else {
            str[i] = keyTable[row1][col2];
            str[i + 1] = keyTable[row2][col1];
        }
    }
}
