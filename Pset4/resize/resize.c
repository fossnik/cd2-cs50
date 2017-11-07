/**
 * Copies a BMP piece by piece, just because.
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize factor infile outfile\n");
        return 1;
    }

    // multiplication factor
    int factor = atoi(argv[1]);

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // print out the BITMAPINFOHEADER
    fprintf(stderr, "\nBITMAPINFOHEADER\nbiSize: %d\nbiWidth: %d\nbiHeight: %d\nbiSizeImage: %d\n",
                    bi.biSize,
                    bi.biWidth,
                    bi.biHeight,
                    bi.biSizeImage);

     // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // modify metadata for the new image size
    bi.biWidth      = bi.biWidth * factor;

    // determine padding for resized image outfile
    int outfilepad  = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    bi.biHeight     = bi.biHeight * factor;
    bi.biSizeImage  = (bi.biWidth * sizeof(RGBTRIPLE) + outfilepad) * abs(bi.biHeight);
    bf.bfSize       = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight / factor; i++)
    {
        // reiterate scanlines for vertical resize
        for (int q = 0; q < factor; q++) {
            // iterate over pixels in scanline (must use original bi.biWidth)
            for (int j = 0; j < bi.biWidth / factor; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile (n times)
                for (int n = 0; n < factor; n++)
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
            // write outfile's padding (if any)
            for (int k = 0; k < outfilepad; k++)
                fputc(0x00, outptr);

            // skip back to beginning of scanline for repeat
            if (q < factor - 1)
                fseek(inptr, -((bi.biWidth/factor) * sizeof(RGBTRIPLE)), SEEK_CUR);
        }
        // skip over infile padding, if any
        fseek(inptr, padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
