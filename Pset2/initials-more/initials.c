// initials // more comfortable

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h> // for toupper

int main(void) {
    string name = get_string();
    bool spacetrip;

    // verify that input is not null
    if (name != NULL) {
        // establishes demarcation between names (from spaces)
        // checks that first char is not a space.
        if (isalpha(name[0])) {
            spacetrip = true;
        } else {
            spacetrip = false;
        }
        // loop through each letter of input
        // until '\0' because that is the end of the string.
        int i = 0;
        while(name[i] != '\0') {
            if (spacetrip == true) {
                printf("%c", toupper(name[i]));
                spacetrip = false;
            }
            // if it's a space then the next character is an initial
            // 32 is the ASCII for a space.
            // checks that next char is indeed an alpha character.
            if ((int) name[i] == 32 && isalpha(name[i+1])) {
                spacetrip = true;
            }
            i++;
        }
    }
    printf("\n");
    return 0;
}