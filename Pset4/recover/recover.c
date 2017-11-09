/**
 * cs50 - pset4 - Recover
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
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

    // seek through input file 512 bytes at a time until end
    for (long byte = 0; fgetc(inptr) != 0xffffffff; byte += 512) {
        _Bool jpeg = 0; // assume not a jpeg (_Bool is a c99 keyword)

        // seek n*512 clusters from file beginning
        fseek(inptr, byte, SEEK_SET);

        // test magic number for jpeg (ff d8 ff & e0-ef)
        if (fgetc(inptr) == 0xff)
            if (fgetc(inptr) == 0xd8)
                if (fgetc(inptr) == 0xff) {
                    int fourth = fgetc(inptr);
                    if (fourth >= 0xe0 && fourth <= 0xef)
                        jpeg = 1;
                }
        // found jpeg magic number
        if (jpeg) {
            printf("JPEG exists at byte %ld \n", byte);
        }
    }

    // close infile
    fclose(inptr);

    // success
    return 0;
}
