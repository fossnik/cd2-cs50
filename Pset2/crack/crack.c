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

void testpasswd(char *passwd, char *salt, char *givenhash);
void recurseq(char *sequence, int index, char *salt, char *givenhash);

int main(int argc, string argv[]) {
    if (!(argc == 2)) {
        printf("Usage: ./crack hash\n");
        return 1;
    }

    char *givenhash = argv[1];

    // establish the salt from first two chars of argv1
    char *salt = malloc(2);
    salt[0] = argv[1][0];
    salt[1] = argv[1][1];

    char *sequence = malloc(4);
    memset(sequence, 0, 4);
    recurseq(sequence, 0, salt, givenhash);

    free(salt);
    free(sequence);

    return 0;
}

void recurseq(char *sequence, int index, char *salt, char *givenhash) {
    for (int i = 0; i < keyspace_size; i++) {
        sequence[index] = keyspace[i];
        if (index == 3) testpasswd(sequence, salt, givenhash);
        else recurseq(sequence, index + 1, salt, givenhash);
    }
}

void testpasswd(char *passwd, char *salt, char *givenhash) {
    char *cryptohash = crypt(passwd, salt);
    if (strcmp(givenhash, cryptohash) == 0) {
        printf("%s\n", passwd);
        return 0;
    }
}