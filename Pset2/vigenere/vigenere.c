// vigenere

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[]) {
    // verify two input arguments
    if (!(argc == 2)) {
        printf("Usage: ./vigenere k\n");
        return 1;
    }
    // verify that the second argment is purely alphabetical
    for (int i = 0, length = strlen(argv[1]); i < length; i++) {
        if (!(isalpha(argv[1][i]))) {
            printf("Usage: ./vigenere k\n");
            return 1;
        }
    }

    string cyphertext = argv[1];

    printf("plaintext:  ");
    string plaintext = get_string();

    printf("ciphertext: ");

    for (int cycle, i = 0, k = 0, textlen = strlen(plaintext), keylen = strlen(cyphertext); i < textlen; i++) {
        // non-alpha characters should be passed through unaltered
        if (!(isalpha(plaintext[i]))) {
            printf("%c", plaintext[i]);
        } else {
            // set rotation quotient by the vigenere cypher key
            // < cyphertext[i%keylen] > cycles key, bounded by key length.
            // < tolower - 97 > will establish both 'A' and 'a' as ROT epoch of 0.
            cycle = tolower(cyphertext[k%keylen]) - 97;
            if (isupper(plaintext[i])) {
                printf("%c", ((plaintext[i] + cycle - 65) % 26) + 65);
            } else {
                printf("%c", ((plaintext[i] + cycle - 97) % 26) + 97);
            }
            // increment the key
            k++;
        }
    }

    printf("\n");
    return 0;
}