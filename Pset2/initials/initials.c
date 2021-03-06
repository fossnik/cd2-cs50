// initials

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h> // for toupper

int main(void) {
    string name = get_string();

    // verify that input is not null
    if (name != NULL) {
        // establishes demarcation between names (from spaces)
        bool spacetrip = true;
        // loop through each letter of input
        for (int i = 0, length = strlen(name); i < length; i++) {
            if (spacetrip == true) {
                printf("%c", toupper(name[i]));
                spacetrip = false;
            }
            // if it's a space then the next character is an initial
            // 32 is the ASCII for a space.
            if ((int) name[i] == 32) {
                spacetrip = true;
            }
        }
    }
    printf("\n");
    return 0;
}