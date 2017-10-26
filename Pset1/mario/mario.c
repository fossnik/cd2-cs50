#include <stdio.h>
#include <cs50.h>

int main(void) {
    // must be initialized outside the do-while loop
    int height;

    // sanity-test user input
    do {
        printf("Height: ");
        height = get_int();
    } while (!(height > -1 && height < 24));

    // loop through columns
    for (int column = height+1; column > 1; column--) {
        // loop through rows within a column
        for (int row = 1; height+2 > row; row++) {
            // print either spaces or poundsigns depending on row progress
            if ( column > row+1 ) { printf(" "); } else { printf("#"); }
        }
        // new line must be between each column
        printf("\n");
    }
}
