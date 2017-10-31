// crack

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

// in order to use crypt
#define _XOPEN_SOURCE
#include <unistd.h>

int main(int argc, string argv[]) {
    // verify two input arguments
    if (!(argc == 2)) {
        printf("Usage: ./crack hash\n");
        return 1;
    }

    printf("\n");
    return 0;
}