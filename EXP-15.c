#include <stdio.h>
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

void shiftCiphertext(char *ciphertext, char *result, int shift) {
    int len = strlen(ciphertext);
    for (int i = 0; i < len; i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            result[i] = (ciphertext[i] - base + shift) % ALPHABET_SIZE + base;
        } else {
            result[i] = ciphertext[i];
        }
    }
    result[len] = '\0';
}

void calculateFrequencies(char *text, double *frequencies) {
    int count[ALPHABET_SIZE] = {0};
    int len = strlen(text);
    int totalLetters = 0;

    for (int i = 0; i < len; i++) {
        if (isalpha(text[i])) {
            count[toupper(text[i]) - 'A']++;
            totalLetters++;
        }
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        frequencies[i] = (double)count[i] / totalLetters * 100;
    }
}

double chiSquaredStatistic(double *observed, double *expected) {
    double chiSquared = 0.0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (expected[i] != 0) {
            chiSquared += ((observed[i] - expected[i]) * (observed[i] - expected[i])) / expected[i];
        }
    }
    return chiSquared;
}

void rankShifts(char *ciphertext, double *chiSquaredValues) {
    char shiftedText[MAX_TEXT_SIZE];
    double frequencies[ALPHABET_SIZE];

    for (int shift = 0; shift < ALPHABET_SIZE; shift++) {
        shiftCiphertext(ciphertext, shiftedText, shift);
        calculateFrequencies(shiftedText, frequencies);
        chiSquaredValues[shift] = chiSquaredStatistic(frequencies, englishFreq);
    }
}

void sortChiSquared(double *chiSquaredValues, int *shifts) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        shifts[i] = i;
    }

    for (int i = 0; i < ALPHABET_SIZE - 1; i++) {
        for (int j = 0; j < ALPHABET_SIZE - i - 1; j++) {
            if (chiSquaredValues[shifts[j]] > chiSquaredValues[shifts[j + 1]]) {
                int temp = shifts[j];
                shifts[j] = shifts[j + 1];
                shifts[j + 1] = temp;
            }
        }
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

    double chiSquaredValues[ALPHABET_SIZE];
    int shifts[ALPHABET_SIZE];

    rankShifts(ciphertext, chiSquaredValues);
    sortChiSquared(chiSquaredValues, shifts);

    char possiblePlaintext[MAX_TEXT_SIZE];
    for (int i = 0; i < topN && i < ALPHABET_SIZE; i++) {
        shiftCiphertext(ciphertext, possiblePlaintext, shifts[i]);
        printf("Possible plaintext with shift %d: %s\n", shifts[i], possiblePlaintext);
    }

    return 0;
}
