/**
 * cs50 - pset4 - Recover
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 1)
    {
        fprintf(stderr, "Usage: ./recover infile\n");
        return 1;
    }
    char *infile = argv[1];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // 0xff

    // close infile
    fclose(inptr);

    // success
    return 0;
}
