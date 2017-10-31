// caesar

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[]) {
    // verify two input arguments
    // verify that 2nd argument is a number (the rotate quotient)
    if (!(argc == 2)) {
        printf("Usage: ./caesar k\n");
        return 1;
    }

    // the second argument is the rotation quotient
    int cycle = atoi(argv[1]);

    printf("plaintext:  ");
    string plaintext = get_string();

    printf("ciphertext: ");
    for (int i = 0, length = strlen(plaintext); i < length; i++) {
        // non-alpha characters should be passed through unaltered
        if (!(isalpha(plaintext[i]))) {
            printf("%c", plaintext[i]);
        } else {
            if (isupper(plaintext[i])) {
                // ASCII uppercase starts at 65 - modulo for bounding
                printf("%c", ((plaintext[i] + cycle - 65) % 26) + 65);
            } else {
                // ASCII lowercase starts at 97
                printf("%c", ((plaintext[i] + cycle - 97) % 26) + 97);
            }
        }
    }

    printf("\n");
    return 0;
}