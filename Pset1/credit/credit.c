// Credit

#include <stdio.h>
#include <cs50.h>

int main(void) {
    // long long is from the CS50 library
    long long plastic;

    // sanity-check user input is purely numerical
    do {
        printf("Number:\n");
        plastic = get_long_long();
    } while (!(plastic > 0));

}