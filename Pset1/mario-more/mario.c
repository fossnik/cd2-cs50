// Mario / More Comfortable

#include <stdio.h>
#include <cs50.h>

int main(void) {
    int height;

    // sanity-check user input
    do {
        printf("Height: ");
        height = get_int();
    } while (!(height > -1 && height < 24));

    // columns loop
    for (int column = height+1; column > 1; column--) {
        // rows loop - left side
        for (int row = 1; height+1 > row; row++) {
            // print either spaces or poundsigns depending on row progress
            if ( column > row+1 ) { printf(" "); } else { printf("#"); }
        }

        // create space in between
        printf("  ");

        // rows loop - right side (row polarity now reversed)
        for (int row = height+1; row > 1; row--) {
            // print either pound sign or we can break the loop,
            // because now we're on the right side and can go to the next line
            if ( column > row ) { break; } else { printf("#"); }
        }

        printf("\n");
    }
}
