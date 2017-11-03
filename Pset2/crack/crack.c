// crack

#include <stdio.h>
#include <cs50.h>
#include <string.h>

// in order to use crypt
#define _XOPEN_SOURCE
#include <unistd.h>
#include <crypt.h>

// string representing keyspace [a-zA-Z0-9./]
static const char keyspace[] =
"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890./";
static const int keyspace_size = sizeof(keyspace);
static const int passwd_size = 4;

int passwerdz(char *prospective_pw, int index, char *salt, char *givenhash);

int main(int argc, char *argv[]) {
    if (!(argc == 2)) {
        printf("Usage: ./crack hash\n");
        return 1;
    }

    char *givenhash = argv[1];

    // establish the salt from first two chars of argv1 (the given hash)
    char *salt = malloc(2);
    salt[0] = argv[1][0];
    salt[1] = argv[1][1];

    char *prospective_pw = malloc(passwd_size);
    memset(prospective_pw, 0, passwd_size);

    return passwerdz(prospective_pw, 0, salt, givenhash);

    // free(salt);
    // free(prospective_pw);

    // return 0;
}

int passwerdz(char *passwd, int index, char *salt, char *givenhash) {
    for (int i = 0; i < keyspace_size; i++) {
        // let tumbler A B C or D be equal to an iterated keyspace value.
        passwd[index] = keyspace[i];
        if (index == 3) {   // base case (a complete sequence of 4)
            // test the prospective password
            if (strcmp(givenhash, crypt(passwd, salt)) == 0) {
                printf("%s\n", passwd);
                return 0;
                // TODO: Why doesn't the program terminate here?
            }
        } else passwerdz(passwd, index + 1, salt, givenhash); // recursion!
    }
    return 1;
}