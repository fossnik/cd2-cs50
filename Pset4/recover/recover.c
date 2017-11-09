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
    char *infile = argv[1];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    fseek(inptr, 0, SEEK_END);      // point to file end
    long raw_size = ftell(inptr);   // record position of end (ie filesize)

    int file_count = 0; // counter to keep track of filenames

    // iterate through input file one block at a time
    for (long byte = 0; byte < raw_size; byte += block_size)
    {
        _Bool signature_detected = 0; // (_Bool is a c99 keyword)

        // set pointer location by distance from file start
        fseek(inptr, byte, SEEK_SET);

        // test magic number for jpeg (ff d8 ff & e0-ef)
        if (fgetc(inptr) == 0xff)
            if (fgetc(inptr) == 0xd8)
                if (fgetc(inptr) == 0xff) {
                    int fourth = fgetc(inptr);
                    if (fourth >= 0xe0 && fourth <= 0xef)
                        signature_detected = 1; // "magic" numbers detected!
                }

        if (signature_detected)
        {
            char fn_buffer[3];  // buffer for filenames
            // write into buffer a filename for the found jpeg
            sprintf(fn_buffer,"%003d.jpg", file_count);
            fprintf(stderr, "File: %s, Position: %ld\n", fn_buffer, ftell(inptr));

            // open output file
            // FILE *outptr = fopen("/dev/null", "w");
            FILE *outptr = fopen(fn_buffer, "w");

            // write output file while
            while (fgetc(inptr) != 0xffffffff)
            {
                fseek(inptr, byte, SEEK_SET); // reset to start of block

                fwrite(&inptr, block_size, 1, outptr); // write one block to outfile

                byte += block_size;

                // prophylacticly test against signature of next file
                if (fgetc(inptr) == 0xff) if (fgetc(inptr) == 0xd8) if (fgetc(inptr) == 0xff)
                {
                    int fourth = fgetc(inptr);
                    if (fourth >= 0xe0 && fourth <= 0xef)
                    {
                        fprintf(stderr, "New Signature Break Position: %ld\n", ftell(inptr));
                        break; // end upon encounter of magic numbers
                    }
                }
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
