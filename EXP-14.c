#include <stdio.h>
#include <string.h>
#include <ctype.h>

void vigenereEncrypt(char *plaintext, int *key, char *ciphertext) {
    int len = strlen(plaintext);
    for (int i = 0; i < len; i++) {
        if (isalpha(plaintext[i])) {
            int shift = toupper(plaintext[i]) - 'A';
            shift = (shift + key[i]) % 26;
            ciphertext[i] = shift + 'A';
        } else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[len] = '\0';
}

void vigenereDecrypt(char *ciphertext, int *key, char *plaintext) {
    int len = strlen(ciphertext);
    for (int i = 0; i < len; i++) {
        if (isalpha(ciphertext[i])) {
            int shift = toupper(ciphertext[i]) - 'A';
            shift = (shift - key[i] + 26) % 26;
            plaintext[i] = shift + 'A';
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[len] = '\0';
}

void findDecryptionKey(char *ciphertext, char *targetPlaintext, int *key) {
    int len = strlen(ciphertext);
    for (int i = 0; i < len; i++) {
        if (isalpha(ciphertext[i]) && isalpha(targetPlaintext[i])) {
            int c = toupper(ciphertext[i]) - 'A';
            int p = toupper(targetPlaintext[i]) - 'A';
            key[i] = (c - p + 26) % 26;
        } else {
            key[i] = 0; // For non-alphabetic characters
        }
    }
}

int main() {
    char plaintext[] = "send more money";
    int keyStream[] = {9, 0, 1, 7, 23, 15, 21, 14, 11, 11, 2, 8, 9};
    int keyStreamLen = sizeof(keyStream) / sizeof(keyStream[0]);
    char ciphertext[50];
    char decryptedText[50];
    int foundKey[50];
    char targetPlaintext[] = "cash not needed";

    // Encrypt the plaintext
    vigenereEncrypt(plaintext, keyStream, ciphertext);
    printf("Encrypted text: %s\n", ciphertext);

    // Find the decryption key
    findDecryptionKey(ciphertext, targetPlaintext, foundKey);
    printf("Found key: ");
    for (int i = 0; i < strlen(targetPlaintext); i++) {
        if (isalpha(targetPlaintext[i])) {
            printf("%d ", foundKey[i]);
        }
    }
    printf("\n");

    // Decrypt the ciphertext with the found key
    vigenereDecrypt(ciphertext, foundKey, decryptedText);
    printf("Decrypted text: %s\n", decryptedText);

    return 0;
}
