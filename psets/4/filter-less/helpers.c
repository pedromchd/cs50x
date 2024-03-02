#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE *img = &image[i][j];
            BYTE rgbtGray = (img->rgbtBlue + img->rgbtGreen + img->rgbtRed) / 3.0 + 0.5;
            img->rgbtBlue = rgbtGray;
            img->rgbtGreen = rgbtGray;
            img->rgbtRed = rgbtGray;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int originalRed = image[i][j].rgbtRed;
            int originalGreen = image[i][j].rgbtGreen;
            int originalBlue = image[i][j].rgbtBlue;

            int sepiaRed = .393 * originalRed + .769 * originalGreen + .189 * originalBlue + 0.5;
            int sepiaGreen = .349 * originalRed + .686 * originalGreen + .168 * originalBlue + 0.5;
            int sepiaBlue = .272 * originalRed + .534 * originalGreen + .131 * originalBlue + 0.5;

            image[i][j].rgbtBlue = sepiaBlue > 255 ? 255 : sepiaBlue;
            image[i][j].rgbtGreen = sepiaGreen > 255 ? 255 : sepiaGreen;
            image[i][j].rgbtRed = sepiaRed > 255 ? 255 : sepiaRed;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, k = width - 1; j < k; j++, k--)
        {
            BYTE tempBlue = image[i][j].rgbtBlue;
            BYTE tempGreen = image[i][j].rgbtGreen;
            BYTE tempRed = image[i][j].rgbtRed;

            image[i][j].rgbtBlue = image[i][k].rgbtBlue;
            image[i][j].rgbtGreen = image[i][k].rgbtGreen;
            image[i][j].rgbtRed = image[i][k].rgbtRed;

            image[i][k].rgbtBlue = tempBlue;
            image[i][k].rgbtGreen = tempGreen;
            image[i][k].rgbtRed = tempRed;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blur[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sumRed = 0;
            float sumGreen = 0;
            float sumBlue = 0;
            int count = 0;
            for (int r = i - 1; r <= i + 1; r++)
            {
                if (r < 0 || r >= height)
                {
                    continue;
                }
                for (int c = j - 1; c <= j + 1; c++)
                {
                    if (c < 0 || c >= width)
                    {
                        continue;
                    }
                    sumRed += image[r][c].rgbtRed;
                    sumGreen += image[r][c].rgbtGreen;
                    sumBlue += image[r][c].rgbtBlue;
                    count++;
                }
            }
            blur[i][j].rgbtRed = (int) (sumRed / count + 0.5);
            blur[i][j].rgbtGreen = (int) (sumGreen / count + 0.5);
            blur[i][j].rgbtBlue = (int) (sumBlue / count + 0.5);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = blur[i][j].rgbtRed;
            image[i][j].rgbtGreen = blur[i][j].rgbtGreen;
            image[i][j].rgbtBlue = blur[i][j].rgbtBlue;
        }
    }

    return;
}
