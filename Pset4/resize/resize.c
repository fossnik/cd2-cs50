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

    /**
     * Elements that require modification for a resized bitmap
     *
     * biSize - The number of bytes required for bitmap =
     * biWidth
     * biHeight
     *
     */

    // print out the BITMAPINFOHEADER
    fprintf(stdout, "\nBITMAPINFOHEADER\nbiSize: %d\nbiWidth: %d\nbiHeight: %d\nbiPlanes: %d\nbiBitCount: %d\nbiCompression: %d\nbiSizeImage: %d\nbiXPelsPerMeter: %d\nbiYPelsPerMeter: %d\nbiClrUsed: %d\nbiClrImportant: %d\n",
                    bi.biSize,
                    bi.biWidth,
                    bi.biHeight,
                    bi.biPlanes,
                    bi.biBitCount,
                    bi.biCompression,
                    bi.biSizeImage,
                    bi.biXPelsPerMeter,
                    bi.biYPelsPerMeter,
                    bi.biClrUsed,
                    bi.biClrImportant);

    // print out BITMAPFILEHEADER
    fprintf(stdout, "\nBITMAPFILEHEADER\nbfType: %d\nbfSize: %d\nbfReserved1: %d\nbfReserved2: %d\nbfOffBits: %d\n", bf.bfType, bf.bfSize, bf.bfReserved1, bf.bfReserved2, bf.bfOffBits);

    /**
     * output diff function diff small large
     *
     * < biWidth: 3
     * > biWidth: 12
     *
     * < biHeight: -3
     * > biHeight: -12
     *
     * < biSizeImage: 36
     * > biSizeImage: 432
     *
     * < bfSize: 90
     * > bfSize: 486
     */

     bi.biWi

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}