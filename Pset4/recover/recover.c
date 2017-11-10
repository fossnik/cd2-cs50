/**
 * cs50 - pset4 - Recover
 */

#include <stdio.h>
#include <stdlib.h>

static const int block_size = 512;

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover infile\n");
        return 1;
    }

    // open input file
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    int fread_buffer[block_size];   // buffer for blocks IO

    int file_count = 0; // counter to keep track of filenames

    // iterate through input file one block at a time
    for (long byte = 0; fgetc(inptr) != 0xffffffff; byte += block_size)
    {
        _Bool signature_detected = 0; // (_Bool is a c99 keyword)
        int fourth;

        // set pointer location by distance from file start
        fseek(inptr, byte, SEEK_SET);

        // filesignature detection (magic numbers ff d8 ff & e0-ef)
        if (fgetc(inptr) == 0xff & fgetc(inptr) == 0xd8 & fgetc(inptr) == 0xff)
        {
            fourth = fgetc(inptr);
            if (fourth >= 0xe0 && fourth <= 0xef)
                signature_detected = 1; // "magic" numbers detected!
        }

        if (signature_detected)
        {
            fseek(inptr, byte, SEEK_SET);   // reset pointer
            char fn_buffer[8];              // buffer for filenames
            // write into buffer a filename for the found jpeg
            sprintf(fn_buffer,"%003d.jpg", file_count);

            // open output file
            FILE *outptr = fopen(fn_buffer, "w");

            // write output file while input not NULL.
            while (fread(fread_buffer, block_size, 1, inptr) != 0)
            {
                fwrite(fread_buffer, block_size, 1, outptr);

                byte += block_size;
                fseek(inptr, byte, SEEK_SET); // reset pointer.
                // avoid running over the next file
                if (fgetc(inptr) == 0xff) if (fgetc(inptr) == 0xd8) if (fgetc(inptr) == 0xff)
                {
                    fourth = fgetc(inptr);
                    if (fourth >= 0xe0 && fourth <= 0xef)
                    {
                        byte -= block_size; // counteract increment
                        break; // end upon encounter of magic numbers
                    }
                }
                fseek(inptr, byte, SEEK_SET); // reset pointer.
            }
            file_count++;
            fclose(outptr); // close outfile - end of jpeg
        }
    }

    // close infile
    fclose(inptr);

    // success
    return 0;
}
