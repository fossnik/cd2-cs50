#include <stdio.h>
#include <cs50.h>
// the cs50 header file

int main(void) {
    int minutes;
    do {
        printf("Minutes: ");
        minutes = get_int();
    } while (minutes < 0);

    // bottles is x12
    int bottles = minutes * 12;
    printf("Bottles: %i\n", bottles);
}
