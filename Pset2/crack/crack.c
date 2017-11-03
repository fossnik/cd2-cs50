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

    // malloc a buffer for the password bruteforce
    char *prospective_pw = malloc(passwd_size);
    memset(prospective_pw, 0, passwd_size);
    // TODO: fix leak - free(prospective_pw);

    // establish the salt from first two chars of argv1 (the given hash)
    char *salt = malloc(2);
    salt[0] = argv[1][0];
    salt[1] = argv[1][1];

    // execute the bruteforce recurser
    return passwerdz(prospective_pw, 0, salt, givenhash);
}

int passwerdz(char *passwd, int tumblr, char *salt, char *givenhash) {
    for (int i = 0; i < keyspace_size; i++) {
        // let tumbler A B C or D be equal to an iterated keyspace value.
        passwd[tumblr] = keyspace[i];
        if (tumblr < 3) passwerdz(passwd, tumblr + 1, salt, givenhash);// recur!
        else { // tumblr == 3 | base case (a complete sequence of 4 chars)
            // test the prospective password
            if (strcmp(givenhash, crypt(passwd, salt)) == 0) {
                printf("%s\n", passwd);
                return 0;
                // TODO: Why doesn't the program terminate here?
            }
        }
    }
    printf("Cracking Failed!\n#Inadequate Seekspace / Big Password#\n");
    return 1;
}