#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function to generate the cipher alphabet from the keyword
void generateCipherAlphabet(char *keyword, char *cipherAlphabet) {
    int alphabetUsed[26] = {0}; // To keep track of which letters have been used
    int index = 0;

    // Add the keyword to the cipher alphabet
    for (int i = 0; keyword[i] != '\0'; i++) {
        char ch = toupper(keyword[i]);
        if (!alphabetUsed[ch - 'A']) {
            cipherAlphabet[index++] = ch;
            alphabetUsed[ch - 'A'] = 1;
        }
    }

    // Add the remaining letters of the alphabet
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (!alphabetUsed[ch - 'A']) {
            cipherAlphabet[index++] = ch;
        }
    }
    cipherAlphabet[index] = '\0'; // Null-terminate the string
}

// Function to encrypt the plaintext using the cipher alphabet
void encrypt(char *plaintext, char *cipherAlphabet, char *ciphertext) {
    for (int i = 0; plaintext[i] != '\0'; i++) {
        if (isalpha(plaintext[i])) {
            char base = isupper(plaintext[i]) ? 'A' : 'a';
            ciphertext[i] = cipherAlphabet[toupper(plaintext[i]) - 'A'];
            if (islower(plaintext[i])) {
                ciphertext[i] = tolower(ciphertext[i]);
            }
        } else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[strlen(plaintext)] = '\0';
}

// Function to decrypt the ciphertext using the cipher alphabet
void decrypt(char *ciphertext, char *cipherAlphabet, char *plaintext) {
    char plainAlphabet[26];
    for (int i = 0; i < 26; i++) {
        plainAlphabet[cipherAlphabet[i] - 'A'] = 'A' + i;
    }

    for (int i = 0; ciphertext[i] != '\0'; i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            plaintext[i] = plainAlphabet[toupper(ciphertext[i]) - 'A'];
            if (islower(ciphertext[i])) {
                plaintext[i] = tolower(plaintext[i]);
            }
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[strlen(ciphertext)] = '\0';
}

int main() {
    char keyword[] = "CIPHER";
    char cipherAlphabet[27]; // 26 letters + 1 null-terminator

    generateCipherAlphabet(keyword, cipherAlphabet);

    printf("Cipher Alphabet: %s\n", cipherAlphabet);

    char plaintext[] = "This is a secret message.";
    char ciphertext[100];
    char decryptedtext[100];

    encrypt(plaintext, cipherAlphabet, ciphertext);
    printf("Encrypted Text: %s\n", ciphertext);

    decrypt(ciphertext, cipherAlphabet, decryptedtext);
    printf("Decrypted Text: %s\n", decryptedtext);

    return 0;
}
