// crack

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

// in order to use crypt
#define _XOPEN_SOURCE
#include <unistd.h>

// string representing keyspace [a-zA-Z0-9./]
static const char keyspace[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890./";

int main(void) {
// int main(int argc, string argv[]) {
    // // verify two input arguments
    // if (!(argc == 2)) {
    //     printf("Usage: ./crack hash\n");
    //     return 1;
    // }

    // char *hash = argv[1];

    // char *salt = hash[0] + hash[1]; // i miss javascript and python so bad.

    // loop through possible passwords

    for(int a = 0; keyspace[a] != '\0'; a++) {
        for(int b = 0; keyspace[b] != '\0'; b++) {
            for(int c = 0; keyspace[c] != '\0'; c++) {
                    printf("%c%c%c\n", keyspace[a],keyspace[b],keyspace[c]);
            }
        }
    }



    // printf("%s\n", salt);

    return 0;
}