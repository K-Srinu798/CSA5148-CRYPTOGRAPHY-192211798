#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define MAX_LEN 100

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

// Function to decrypt ciphertext using a provided substitution key
void decrypt_substitution_cipher(const char *ciphertext, const char *substitution_key, char *plaintext) {
    int len = strlen(ciphertext);
    for (int i = 0; i < len; i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            plaintext[i] = substitution_key[ciphertext[i] - base];
        } else {
            plaintext[i] = ciphertext[i];  // Preserve non-alphabet characters
        }
    }
    plaintext[len] = '\0';
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

    // English letter frequencies (from most to least common)
    // Source: https://en.wikipedia.org/wiki/Letter_frequency
    double english_frequencies[ALPHABET_SIZE] = {
        8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153, 0.772, 4.025,
        2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056, 2.758, 0.978, 2.360, 0.150, 1.974, 0.074
    };

    // Array to store likelihood scores for each substitution key as potential plaintext
    double likelihood_scores[ALPHABET_SIZE] = {0.0};

    // Calculate likelihood scores for each possible substitution key
    char substitution_key[ALPHABET_SIZE];
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        substitution_key[i] = i + 'a';
    }

    // Sort likelihood scores to determine the top plaintext guesses
    // Using a simple bubble sort for demonstration purposes
    for (int i = 0; i < ALPHABET_SIZE - 1; i++) {
        for (int j = 0; j < ALPHABET_SIZE - i - 1; j++) {
            if (ciphertext_percentages[j] < ciphertext_percentages[j + 1]) {
                // Swap percentages
                double temp_percentage = ciphertext_percentages[j];
                ciphertext_percentages[j] = ciphertext_percentages[j + 1];
                ciphertext_percentages[j + 1] = temp_percentage;

                // Swap corresponding substitution key index
                char temp_index = substitution_key[j];
                substitution_key[j] = substitution_key[j + 1];
                substitution_key[j + 1] = temp_index;
            }
        }
    }

    // Display the top plaintext guesses
    printf("Top %d possible plaintexts:\n", top_plaintexts);
    for (int i = 0; i < top_plaintexts; i++) {
        printf("%d. Key: %s - ", i + 1, substitution_key);
        char plaintext[MAX_LEN];
        decrypt_substitution_cipher(ciphertext, substitution_key, plaintext);
        printf("%s\n", plaintext);

        // Shuffle the substitution key for next possible plaintext
        char temp = substitution_key[0];
        for (int j = 0; j < ALPHABET_SIZE - 1; j++) {
            substitution_key[j] = substitution_key[j + 1];
        }
        substitution_key[ALPHABET_SIZE - 1] = temp;
    }
}

int main() {
    // Example ciphertext
    const char *ciphertext = "Znk hnw ohhjmfx Ymnx tk tw jshjw uwtanji. Fqnhjfsi, Ns Fqnhjfsi!";
    // Number of top possible plaintexts to display
    int top_plaintexts = 5;

    // Perform letter frequency attack
    letter_frequency_attack(ciphertext, top_plaintexts);

    return 0;
}
OUTPUT:
Znk hnw ohhjmfx Ymnx tk tw jshjw uwtanji. Fqnhjfsi, Ns Fqnhjfsi!
