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
    long raw_size = ftell(inptr);   // mark pointer position (ie filesize)
    fseek(inptr, 0, SEEK_SET);      // return point back to start

    int file_count = 0; // counter to keep track of filenames

    // seek through input file one block at a time
    for (long byte = 0; ftell(inptr) < raw_size; byte += block_size) {
        _Bool signature_detected = 0; // (_Bool is a c99 keyword)

        // seek to the start of next block (set pointer location)
        fseek(inptr, byte, SEEK_SET);

        // test magic number for jpeg (ff d8 ff & e0-ef)
        if (fgetc(inptr) == 0xff)
            if (fgetc(inptr) == 0xd8)
                if (fgetc(inptr) == 0xff) {
                    int fourth = fgetc(inptr);
                    if (fourth >= 0xe0 && fourth <= 0xef)
                        signature_detected = 1; // "magic" numbers detected!
                }

        if (signature_detected) {
            // write into buffer a filename for the found jpeg
            char fn_buffer[3];
            sprintf(fn_buffer,"%d.jpeg", file_count);

            // open output file
            FILE *outptr = fopen(fn_buffer, "w");
            if (outptr == NULL) {
                fclose(inptr);
                fprintf(stderr, "Could not create %s.\n", fn_buffer);
                return 3;
            }

            // write output file not at end
            while (fgetc(inptr) != 0xffffffff) {
                // write write write
                fwrite(&inptr, block_size, 1, outptr);

                byte += block_size;

                // prophylacticly test against signature of next file
                if (fgetc(inptr) == 0xff) if (fgetc(inptr) == 0xd8) if (fgetc(inptr) == 0xff)
                {
                    int fourth = fgetc(inptr);
                    if (fourth >= 0xe0 && fourth <= 0xef)
                        break; // end upon encounter of magic numbers
                }
            }
            fclose(outptr); // close outfile - end of jpeg
            file_count++;
        }

    }

    // close infile
    fclose(inptr);

    // success
    return 0;
}
