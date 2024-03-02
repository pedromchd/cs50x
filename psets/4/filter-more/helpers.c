#include "helpers.h"

#include <stdio.h>

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
    return;
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
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
