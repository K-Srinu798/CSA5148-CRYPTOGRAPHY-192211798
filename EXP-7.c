#include <stdio.h>
#include <string.h>
#include <ctype.h>

void frequencyAnalysis(char *ciphertext, int freq[256]) {
    int length = strlen(ciphertext);
    for (int i = 0; i < length; i++) {
        char ch = ciphertext[i];
        if (isprint(ch)) {
            freq[(int)ch]++;
        }
    }
}

void displayFrequencies(int freq[256]) {
    printf("Character Frequencies:\n");
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            printf("'%c' : %d\n", i, freq[i]);
        }
    }
}

void substitute(char *ciphertext, char *substitution) {
    int length = strlen(ciphertext);
    char result[length + 1];
    
    for (int i = 0; i < length; i++) {
        char ch = ciphertext[i];
        if (isprint(ch)) {
            result[i] = substitution[(int)ch] ? substitution[(int)ch] : ch;
        } else {
            result[i] = ch;
        }
    }
    result[length] = '\0';
    printf("Substituted text:\n%s\n", result);
}

int main() {
    char ciphertext[] = "53‡‡†305))6*;4826)4‡.)4‡);806*;48†8¶60))85;;]8*;:‡*8†83"
                        "(88)5*†;46(;88*96*?;8)*‡(;485);5*†2:*‡(;4956*2(5*—4)8¶8*"
                        ";4069285);)6†8)4‡‡;1(‡9;48081;8:8‡1;48†85;4)485†528806*81"
                        "(‡9;48;(88;4(‡?34;48)4‡;161;:188;‡?;";
                        
    int freq[256] = {0};
    frequencyAnalysis(ciphertext, freq);
    displayFrequencies(freq);

    char substitution[256] = {0};
    substitution['‡'] = 'E';
    substitution['8'] = 'T';
    // Add more substitutions based on analysis...

    substitute(ciphertext, substitution);

    return 0;
}
