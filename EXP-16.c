#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define MAX_TEXT_SIZE 1024

// English letter frequency (relative frequency)
double englishFreq[ALPHABET_SIZE] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966,
    0.153, 0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987,
    6.327, 9.056, 2.758, 0.978, 2.360, 0.150, 1.974, 0.074
};

void calculateFrequencies(char *text, double *frequencies, int *count) {
    int totalLetters = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        count[i] = 0;
    }

    int len = strlen(text);
    for (int i = 0; i < len; i++) {
        if (isalpha(text[i])) {
            int idx = toupper(text[i]) - 'A';
            count[idx]++;
            totalLetters++;
        }
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        frequencies[i] = (double)count[i] / totalLetters * 100;
    }
}

void mapFrequencies(int *cipherCount, char *mapping) {
    int englishOrder[ALPHABET_SIZE];
    int cipherOrder[ALPHABET_SIZE];

    // Initialize order arrays
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        englishOrder[i] = i;
        cipherOrder[i] = i;
    }

    // Sort the English frequencies
    for (int i = 0; i < ALPHABET_SIZE - 1; i++) {
        for (int j = 0; j < ALPHABET_SIZE - i - 1; j++) {
            if (englishFreq[englishOrder[j]] < englishFreq[englishOrder[j + 1]]) {
                int temp = englishOrder[j];
                englishOrder[j] = englishOrder[j + 1];
                englishOrder[j + 1] = temp;
            }
        }
    }

    // Sort the cipher frequencies
    for (int i = 0; i < ALPHABET_SIZE - 1; i++) {
        for (int j = 0; j < ALPHABET_SIZE - i - 1; j++) {
            if (cipherCount[cipherOrder[j]] < cipherCount[cipherOrder[j + 1]]) {
                int temp = cipherOrder[j];
                cipherOrder[j] = cipherOrder[j + 1];
                cipherOrder[j + 1] = temp;
            }
        }
    }

    // Create the mapping
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        mapping[cipherOrder[i]] = englishOrder[i] + 'A';
    }
}

void decryptWithMapping(char *ciphertext, char *plaintext, char *mapping) {
    int len = strlen(ciphertext);
    for (int i = 0; i < len; i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            plaintext[i] = mapping[toupper(ciphertext[i]) - 'A'] + (base - 'A');
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[len] = '\0';
}

void performFrequencyAttack(char *ciphertext, int topN) {
    double cipherFrequencies[ALPHABET_SIZE];
    int cipherCount[ALPHABET_SIZE];
    char mapping[ALPHABET_SIZE];
    char possiblePlaintext[MAX_TEXT_SIZE];

    calculateFrequencies(ciphertext, cipherFrequencies, cipherCount);
    mapFrequencies(cipherCount, mapping);

    printf("Most likely plaintexts:\n");
    for (int i = 0; i < topN; i++) {
        decryptWithMapping(ciphertext, possiblePlaintext, mapping);
        printf("Possible plaintext %d: %s\n", i + 1, possiblePlaintext);

        // Rotate the mapping to generate next likely plaintext
        char temp = mapping[0];
        for (int j = 0; j < ALPHABET_SIZE - 1; j++) {
            mapping[j] = mapping[j + 1];
        }
        mapping[ALPHABET_SIZE - 1] = temp;
    }
}

int main() {
    char ciphertext[MAX_TEXT_SIZE];
    printf("Enter the ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0'; // Remove the newline character

    int topN;
    printf("Enter the number of top possible plaintexts you want: ");
    scanf("%d", &topN);

    performFrequencyAttack(ciphertext, topN);

    return 0;
}
