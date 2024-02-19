// Julia Enriquetto de Brito
#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    do // loop for height
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8); // height can´t be anything but a number of 1 to 8

    for (int row = 0; row < height; row++) // loop for rows
    {
        for (int emptySpace = 0; emptySpace < height - row - 1; emptySpace++)
            printf(" "); // loop for empty spaces according to height
        for (int column = 0; column <= row; column++)
        {
            printf("#"); // loop for the pyramid
        }
        printf("\n");
    }
}
