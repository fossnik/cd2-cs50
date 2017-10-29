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

        // rows loop - right side
        for (int row = 1; height+1 > row; row++) {
            // print either spaces or poundsigns depending on row progress
            if ( column < row+1 ) { printf(" "); } else { printf("#"); }
        }

        printf("\n");
    }
}
