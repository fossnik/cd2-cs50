#include <stdio.h>
#include <cs50.h>
// the cs50 header file

int main(void) {
    printf("Minutes: ");
    int minutes = get_int();

    // bottles is x12
    int bottles = minutes * 12;
    printf("Bottles: %i\n", bottles);
}
