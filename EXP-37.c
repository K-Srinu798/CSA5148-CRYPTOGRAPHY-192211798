#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

// Function to calculate letter frequencies in a given string
void calculate_letter_frequency(const char *text, int *frequency) {
    int i;
    for (i = 0; i < strlen(text); i++) {
        char ch = tolower(text[i]);
        if (isalpha(ch)) {
            frequency[ch - 'a']++;
        }
    }
}

// Function to perform letter frequency attack on monoalphabetic substitution cipher
void letter_frequency_attack(const char *ciphertext, int top_plaintexts) {
    int ciphertext_length = strlen(ciphertext);
    int ciphertext_frequency[ALPHABET_SIZE] = {0};
    double ciphertext_percentages[ALPHABET_SIZE] = {0.0};

    // Calculate letter frequencies in ciphertext
    calculate_letter_frequency(ciphertext, ciphertext_frequency);

    // Calculate percentages
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        ciphertext_percentages[i] = (double)ciphertext_frequency[i] / ciphertext_length * 100.0;
    }

    // Array to store likelihood scores for each letter as potential plaintext
    double likelihood_scores[ALPHABET_SIZE] = {0.0};

    // English letter frequencies (from most to least common)
    // Source: https://en.wikipedia.org/wiki/Letter_frequency
    double english_frequencies[ALPHABET_SIZE] = {
        8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153, 0.772, 4.025,
        2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056, 2.758, 0.978, 2.360, 0.150, 1.974, 0.074
    };

    // Calculate likelihood scores for each letter as potential plaintext
    for (int shift = 0; shift < ALPHABET_SIZE; shift++) {
        double score = 0.0;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            int index = (i + shift) % ALPHABET_SIZE;
            score += english_frequencies[i] * ciphertext_percentages[index];
        }
        likelihood_scores[shift] = score;
    }

    // Sort likelihood scores to determine the top plaintext guesses
    // Using a simple bubble sort for demonstration purposes
    for (int i = 0; i < ALPHABET_SIZE - 1; i++) {
        for (int j = 0; j < ALPHABET_SIZE - i - 1; j++) {
            if (likelihood_scores[j] < likelihood_scores[j + 1]) {
                // Swap scores
                double temp_score = likelihood_scores[j];
                likelihood_scores[j] = likelihood_scores[j + 1];
                likelihood_scores[j + 1] = temp_score;

                // Swap corresponding plaintext index
                int temp_index = j;
                j = j + 1;
                j = temp_index;
            }
        }
    }

    // Display the top plaintext guesses
    printf("Top %d possible plaintexts:\n", top_plaintexts);
    for (int i = 0; i < top_plaintexts; i++) {
        printf("%d. Shift %d: ", i + 1, i);
        for (int j = 0; j < ciphertext_length; j++) {
            char ch = ciphertext[j];
            if (isalpha(ch)) {
                // Decrypt using current shift
                int index = (ch - 'a' - i + ALPHABET_SIZE) % ALPHABET_SIZE;
                printf("%c", 'a' + index);
            } else {
                printf("%c", ch); // Non-alphabet characters
            }
        }
        printf("\n");
    }
}

int main() {
    // Example ciphertext
    const char *ciphertext = "Ymj vznwr zxnsl oqtem vxj fsi tajw ymjw fxjqfwj nx yt rjxxflj tk ymj htijw ymtzx";
    // Number of top possible plaintexts to display
    int top_plaintexts = 5;

    // Perform letter frequency attack
    letter_frequency_attack(ciphertext, top_plaintexts);

    return 0;
}
OUTPUT:
Ymj vznwr zxnsl oqtem vxj fsi tajw ymjw fxjqfwj nx yt rjxxflj tk ymj htijw ymtzx
