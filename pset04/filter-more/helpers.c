// Julia Enriquetto De Brito - CS50 2024
#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // 0 0 0 -> black; 255 255 255 -> white

    // average from rgb for 1 pixel
    // so, if i had the originals rgbtRed = 50, rgbtGreen = 190, rgbtBlue = 90: i would do 50 + 190 + 90 / 3 = 110 (average)
    // then the new colors are rgtRed = 110, rgbtGreen = 110, rgbtBlue = 110 shade of gray

    // for each pixel calculate the average pixel value
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;

            int average = round((red + green + blue) / 3.0);

            // set each color value to the average value
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // swap pixels on horizontally opposite sides (think about the rows)
    RGBTRIPLE tmpImage;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // swaping
            tmpImage = image[i][j];
            image[i][j] = image[i][width - j - 1]; // the other half
            image[i][width - j - 1] = tmpImage;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // create a copy
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // average red, average green, average blue = sum to the middle pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumRed = 0;
            int sumGreen = 0;
            int sumBlue = 0;
            int totalPixels = 0;

            // count pixels around a given pixel
            // this loop will iterate over the 3x3 grid centered on the pixel (i, j)
            for (int k = i - 1; k <= i + 1; k++)
            {
                for (int l = j - 1; l <= j + 1; l++)
                {
                    // checking if the pixel we're trying to acess is within the image boundaries
                    if (k >= 0 && l >= 0 && k < height && l < width)
                    {
                        sumRed += copy[k][l].rgbtRed;
                        sumGreen += copy[k][l].rgbtGreen;
                        sumBlue += copy[k][l].rgbtBlue;
                        totalPixels++;
                    }
                }
            }
            // sum all the rgb and their averages
            image[i][j].rgbtRed = round((float) sumRed / totalPixels);
            image[i][j].rgbtGreen = round((float) sumGreen / totalPixels);
            image[i][j].rgbtBlue = round((float) sumBlue / totalPixels);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    // *this could be useful for artificial intelligence (sobel operator)

    // highlight all of the boundaries between objects

    // how can we detect a boundary line?
    // because the colors on the left side of the line are different from the colors on the right side of the line

    // convolutional matrix gx and gy determinate if there is some sort of change in color or boundary between objects as we move in
    // x or y direction sobel operator kernels
    int gx[3][3] = {{-1, 0, 1}, {2, 0, 2}, {-1, 0, 1}};

    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // 3x3 box around the pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int gxRed = 0, gxGreen = 0, gxBlue = 0;
            int gyRed = 0, gyGreen = 0, gyBlue = 0;

            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    int ni = i + k; // is the row index of the neighboring pixel.
                    int nj = j + l; // is the column index of the neighboring pixel.

                    // for pixels at the border, treat any pixel past the border as having all 0 values (solid black)
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        // compute gx and gy for each channel of red, green, and blue
                        gxRed += gx[k + 1][l + 1] * image[ni][nj].rgbtRed;
                        gxGreen += gx[k + 1][l + 1] * image[ni][nj].rgbtGreen;
                        gxBlue += gx[k + 1][l + 1] * image[ni][nj].rgbtBlue;
                        gyRed += gy[k + 1][l + 1] * image[ni][nj].rgbtRed;
                        gyGreen += gy[k + 1][l + 1] * image[ni][nj].rgbtGreen;
                        gyBlue += gy[k + 1][l + 1] * image[ni][nj].rgbtBlue;
                    }
                }
            }

            // compute each new channel value as the square root of Gx² + Gy² to generate a final value for each color
            int finalRed = round(sqrt(gxRed * gxRed + gyRed * gyRed));
            int finalGreen = round(sqrt(gxGreen * gxGreen + gyGreen * gyGreen));
            int finalBlue = round(sqrt(gxBlue * gxBlue + gyBlue * gyBlue));

            // verify if final values can only take on int values from 0 to 255 and create a image copy for all the rgb
            copy[i][j].rgbtRed = (finalRed > 255) ? 255 : finalRed;
            copy[i][j].rgbtGreen = (finalGreen > 255) ? 255 : finalGreen;
            copy[i][j].rgbtBlue = (finalBlue > 255) ? 255 : finalBlue;
        }
    }

    // original image to imageCopy
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }
    return;
}
