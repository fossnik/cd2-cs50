/**
 * Resize - More Comfortable
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // // ensure proper usage
    // if (argc != 4)
    // {
    //     fprintf(stderr, "Usage: ./resize resize_factor infile outfile\n");
    //     return 1;
    // }

    // // remember float resize value
    // float resize = atof(argv[1]);

    // // remember filenames
    // char *infile = argv[2], *outfile = argv[3];


    /** Alternative Usage Regime for Testing
     *
     *  2 args - Test Resize
     *      let infile = large.bmp
     *      let outfile = _test.bmp
     *
     *  1 arg - Just Print Header Data
     *      let outfile = /dev/null
     */
    char *infile, *outfile;
    float resize = atof(argv[1]);

    switch ( argc )
    {
        case 4: // manually define all arguments & break
            infile = argv[2];
            outfile = argv[3];
            break;
        case 3:
            infile = argv[2];
            outfile = "/dev/null";
            break;
        case 2:
            infile = "large.bmp";
            outfile = "_test.bmp";
            break;
        default:
            fprintf(stderr, "Usage: ./resize resize_factor <infile> <outfile>\n");
            return 1;
    }

    fprintf(stdout, "infile = %s   \toutfile = %s\n", infile, outfile);

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
    BITMAPFILEHEADER bf, newbf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi, newbi;
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

    // modify metadata for the new image size - cast to int
    newbi.biWidth = (int)(bi.biWidth * resize);

    // determine padding for resized image outfile
    int outfilepad      = (4 - (newbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    newbi.biHeight      = bi.biHeight * resize;
    newbi.biSizeImage   = (newbi.biWidth * sizeof(RGBTRIPLE) + outfilepad) * abs(newbi.biHeight);
    newbf.bfSize        = newbi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    /**
     * small.jpg                large.jpg
     * -: old header :-         -: new header:-
     * Width     =  3           Width     =  12
     * Height    =  -3          Height    =  -12
     * SizeImage =  36          SizeImage =  432
     * bfSize    =  90          bfSize    =  486
     */

    // test header mods
    fprintf(stderr,
        "-: old header :-  \t  -: new header:-\n"
        "Width     =  %d   \t  Width     =  %d\n"
        "Height    =  %d   \t  Height    =  %d\n"
        "SizeImage =  %d   \t  SizeImage =  %d\n"
        "bfSize    =  %d   \t  bfSize    =  %d\n",
        bi.biWidth, newbi.biWidth,
        bi.biHeight, newbi.biHeight,
        bi.biSizeImage, newbi.biSizeImage,
        bf.bfSize, newbf.bfSize
    );

    // write outfile's BITMAPFILEHEADER
    fwrite(&newbf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&newbi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // establish how many bytes to fseek over during image shrinking.
    int skip_over_bytes;
    if (resize < 1) { skip_over_bytes = (1 / resize) * sizeof(RGBTRIPLE); }
    else { skip_over_bytes = 1 * sizeof(RGBTRIPLE); }

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in each scanline
        for (int j = 0; j < bi.biWidth; j++) // temporary storage
        {
            // temporary storage (&tripal points to structs which represent RGB pixels)
            RGBTRIPLE triple;
            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

            // fseek over some pixels (only if downsizing the image)
            if (resize < 1 && j + 1 < bi.biWidth)
                fseek(inptr, skip_over_bytes, SEEK_CUR);
        }
        // skip over input file padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // write outfile's padding (if any)
        for (int k = 0, pad = outfilepad; k < pad; k++)
            fputc(0x00, outptr);

        // skip ahead in the input relative to resize factor
        if (skip_over_bytes > 1)
            fseek(inptr, skip_over_bytes * (bi.biWidth + padding), SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
