#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int TOTAL_JPG = 512; // JPEG block size in memory card

int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open the memory card
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open.\n");
        return 2;
    }

    // to read data from a file, temporarily store the data in a "buffer"
    uint8_t buffer[TOTAL_JPG];

    // output file
    FILE *cardOut = NULL;
    int counter = 0;
    char filename[9]; // Enough space for "###.jpg" + '\0'

    // While there's still data left to read from the memory card
    while (fread(buffer, 1, TOTAL_JPG, card) == TOTAL_JPG)
    {
        // represents a beggining of a JPEG file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Create JPG from the data
            // filename format: ###.jpg, starting at 000.jpg
            sprintf(filename, "%03i.jpg", counter);
            if (cardOut != NULL)
            {
                fclose(cardOut);
            }

            // open output file for writing
            cardOut = fopen(filename, "w");
            if (cardOut == NULL)
            {
                return 3;
            }
            // count number of images found
            counter++;
        }

        if (cardOut != NULL)
        {
            fwrite(buffer, 1, TOTAL_JPG, cardOut);
        }
    }

    if (cardOut != NULL)
    {
        fclose(cardOut);
    }
    fclose(card);

    return 0;
}
