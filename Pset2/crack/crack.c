// crack

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

// in order to use crypt
#define _XOPEN_SOURCE
#include <unistd.h>

int main(void) {
// int main(int argc, string argv[]) {
    // // verify two input arguments
    // if (!(argc == 2)) {
    //     printf("Usage: ./crack hash\n");
    //     return 1;
    // }

    // char *hash = argv[1];

    // char *salt = hash[0] + hash[1]; // i miss javascript and python so bad.

    // string representing keyspace [a-zA-Z0-9./]
    char keyspace[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890./";
    // loop through possible passwords
    char *ptr;
    ptr = keyspace;

    for(int i = 0; keyspace[i] != '\0'; i++) {
        printf("%c", keyspace[i]);
    }



    // printf("%s\n", salt);

    return 0;
}