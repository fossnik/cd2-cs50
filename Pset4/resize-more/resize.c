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
    //     fprintf(stderr, "Usage: ./resize factor infile outfile\n");
    //     return 1;
    // }

    // // remember float resize value
    // float resize = atof(argv[1]);

    // // remember filenames
    // char *infile = argv[2], *outfile = argv[3];


    /** Alternative Usage Regime for Testing
     */
    if (!(argc < 5 && argc > 1)) {
        fprintf(stderr, "\n"
        "!!~ Alternative Usage Regime for Testing ~!!\n\n"
        "Usage: ./resize factor <infile> <outfile>\n\n"
            "3 args - Normal Mode\n\t"
                "./resize factor infile outfile\n\n"
            "2 args - Test Resize\n\t"
                "let infile = large.bmp || small.bmp\n\t"
                "let outfile = _test.bmp\n\n"
            "1 arg - Just Print Header Data\n\t"
                "let outfile = /dev/null\n\n"
        );
        return 1;
    }

    char *infile, *outfile;
    float resize = atof(argv[1]);

    switch ( argc ) {
        case 4:
            infile = argv[2];
            outfile = argv[3];
            break;
        case 3:
            printf("\t~: Header Print Mode :~\n");
            infile = argv[2];
            outfile = "/dev/null";
            break;
        case 2:
            printf("\t~: Algorithm Test Mode :~\n");
            if (resize < 1) infile = "large.bmp";
            else infile = "small.bmp";
            outfile = "_test.bmp";
            break;
        default:
            return 1;
    }

    fprintf(stdout, "infile = %s   \t  outfile = %s\n", infile, outfile);

    /** End of Alternative Usage Regime
     */

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL) {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL) {
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

    // make a copies of the File and Info headers
    newbf = bf, newbi = bi;

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

    // Display Pertinent Header Data
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

    // determine padding for scanlines (of infile)
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // establish how many bytes to fseek over during image shrinking.
    int skip_over_pixels, skip_over_scanlines;

    if (resize < 1) // shrinking image
    {
        skip_over_pixels = (1 / resize) * sizeof(RGBTRIPLE);
        skip_over_scanlines = skip_over_pixels * (bi.biWidth + padding);

        /**
         *   123456789abc
         * 1 XXXXXXXXXXXX XXX
         * 2 XXXXXXXXXXXX X X
         * 3 XXXXXXXXXXXX XXX
         * 4 XXXXXXXXXXXX
         * 5 XXXX    XXXX
         * 6 XXXX    XXXX
         * 7 XXXX    XXXX
         * 8 XXXX    XXXX
         * 9 XXXXXXXXXXXX
         * a XXXXXXXXXXXX
         * b XXXXXXXXXXXX
         * c XXXXXXXXXXXX
         */

        // iterate over 3 scanlines (or however many lines the shrunk image will have)
        for (int scanline = abs(newbi.biHeight); scanline > 0; scanline--)
        {
            // temporary storage (&triple points to structs which represent RGB pixels)
            RGBTRIPLE triple;
            int red = 0, green = 0, blue = 0;

            // iterate over 3 pixels comprising the scanline for the outfile
            for (int outline = newbi.biWidth; outline > 0; outline--)
            {
                // iterate over 4 of 12 pixels from the infile
                for (int pixel = newbi.biWidth + 1; pixel > 0; pixel--)
                {
                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                    red   += triple.rgbtRed;
                    green += triple.rgbtGreen;
                    blue  += triple.rgbtBlue;
                }

                // average 4 verticle pixels
                triple.rgbtRed   = red / newbi.biWidth;
                triple.rgbtGreen = green / newbi.biWidth;
                triple.rgbtBlue  = blue / newbi.biWidth;

                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }

            // skip over input file padding, if any
            fseek(inptr, padding, SEEK_CUR);

            // write outfile's padding (if any)
            for (int k = 0; k < outfilepad; k++)
                fputc(0x00, outptr);

            // skip over scanlines in the input file according to skip_over_scanlines
            // or possibly don't do that, and instead try to find a way to average it in the verticle dimension
            fseek(inptr, skip_over_scanlines, SEEK_CUR);
        }
    }
    else // enlarging image
    {
        // iterate over infile's scanlines
        for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
        {
            // reiterate scanlines for vertical resize
            for (int q = 0; q < resize; q++) {
                // iterate over pixels in scanline (must use original bi.biWidth)
                for (int j = 0; j < bi.biWidth; j++)
                {
                    // temporary storage
                    RGBTRIPLE triple;

                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                    // write RGB triple to outfile (n times)
                    for (int n = 0; n < resize; n++)
                        fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
                // write outfile's padding (if any)
                for (int k = 0; k < outfilepad; k++)
                    fputc(0x00, outptr);

                // skip back to beginning of scanline for repeat
                if (q < resize - 1)
                    fseek(inptr, -((bi.biWidth) * sizeof(RGBTRIPLE)), SEEK_CUR);
            }
            // skip over infile padding, if any
            fseek(inptr, padding, SEEK_CUR);
        }
    }

    // close infile & outfile
    fclose(inptr);
    fclose(outptr);

    // success
    return 0;
}
