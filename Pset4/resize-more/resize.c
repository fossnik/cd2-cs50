/**
 * Resize - More Comfortable
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    if (!(argc < 5 && argc > 1))
    {
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

    switch ( argc )
    {
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

    // make a copies of the File and Info headers
    BITMAPFILEHEADER newbf = bf;
    BITMAPINFOHEADER newbi = bi;

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

    // determine padding for outfile (our resized image)
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

    // write outfile's edited copy of the original BITMAPFILEHEADER
    fwrite(&newbf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's edited copy of the original BITMAPINFOHEADER
    fwrite(&newbi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines (of infile)
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // establish how many bytes to fseek over during image shrinking.
    int skip_over_pixels, skip_over_scanlines;

    if (resize < 1)
    { // shrinking image
        skip_over_pixels = sizeof(RGBTRIPLE) * (1 / resize);
        skip_over_scanlines = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) * (1 / resize - 1);

        /**
         *   123456789ABC 123456 123
         * 1 ############ ###### ###
         * 2 ############ ###### # #
         * 3 ############ ##  ## ###
         * 4 ############ ##  ##
         * 5 ####    #### ######
         * 6 ####    #### ######
         * 7 ####    ####
         * 8 ####    ####
         * 9 ############
         * A ############
         * B ############
         * C ############
         */

        // iterate vertically over 3 scanlines (or however many lines the shrunk image will have)
        for (int scanline = abs(newbi.biHeight); scanline > 0; scanline--)
        {
            // temporary storage (&triple points to structs which represent RGB pixels)
            RGBTRIPLE triple;

            // horizontally iterating, build the 3 pixel wide scanline for the outfile
            for (int segment = newbi.biWidth; segment > 0; segment--)
            {
                int tR = 0, tG = 0, tB = 0;
                // each of the 3 pixels in the outfile scanline is derived after
                // iterating over a 4 pixel segment of the present infile scanline
                for (int pixel = 1 / resize; pixel > 0; pixel--)
                {
                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                    // sum pixels up
                    tR += triple.rgbtRed     * triple.rgbtRed;
                    tG += triple.rgbtGreen   * triple.rgbtGreen;
                    tB += triple.rgbtBlue    * triple.rgbtBlue;
                }
                // average 4 infile pixels to make 1 outfile pixel
                triple.rgbtRed   = sqrt(tR / (1 / resize));
                triple.rgbtGreen = sqrt(tG / (1 / resize));
                triple.rgbtBlue  = sqrt(tB / (1 / resize));

                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
            // skip over input file padding, if any
            fseek(inptr, padding, SEEK_CUR);

            // write outfile's padding (if any)
            for (int k = 0; k < outfilepad; k++)
                fputc(0x00, outptr);

            // skip over scanlines in the input file according to skip_over_scanlines
            // or possibly don't do that, and instead try to find a way to average it in the vertical dimension
            fseek(inptr, skip_over_scanlines, SEEK_CUR);
        }
    } else // enlarging image
    {
        // iterate over infile's scanlines
        for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
        {
            // copy scanlines for vertical resize
            for (int q = 0; q < resize; q++)
            {
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
