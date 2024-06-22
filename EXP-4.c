#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function to encrypt the plaintext using a polyalphabetic substitution cipher
void encrypt(char plaintext[], char keyword[], char ciphertext[]) {
    int textLen = strlen(plaintext);
    int keyLen = strlen(keyword);
    int keyIndex = 0;

    for (int i = 0; i < textLen; i++) {
        if (isalpha(plaintext[i])) {
            char base = isupper(plaintext[i]) ? 'A' : 'a';
            ciphertext[i] = ((plaintext[i] - base) + (toupper(keyword[keyIndex % keyLen]) - 'A')) % 26 + base;
            keyIndex++;
        } else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[textLen] = '\0';
}

// Function to decrypt the ciphertext using a polyalphabetic substitution cipher
void decrypt(char ciphertext[], char keyword[], char plaintext[]) {
    int textLen = strlen(ciphertext);
    int keyLen = strlen(keyword);
    int keyIndex = 0;

    for (int i = 0; i < textLen; i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            plaintext[i] = ((ciphertext[i] - base) - (toupper(keyword[keyIndex % keyLen]) - 'A') + 26) % 26 + base;
            keyIndex++;
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[textLen] = '\0';
}

int main() {
    char plaintext[100], keyword[100], ciphertext[100];
    
    printf("Enter the plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0';  // Remove newline character

    printf("Enter the keyword: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = '\0';  // Remove newline character

    encrypt(plaintext, keyword, ciphertext);
    printf("Encrypted text: %s\n", ciphertext);

    char decryptedtext[100];
    decrypt(ciphertext, keyword, decryptedtext);
    printf("Decrypted text: %s\n", decryptedtext);

    return 0;
}
