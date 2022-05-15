#include "helpers.h"
#include <stddef.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{ 
    unsigned int average;
    // tuch each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            average = (int)((image[i][j].rgbtRed + image[i][j].rgbtBlue + 
                             image[i][j].rgbtGreen) / 3.0f + .5f);
                             
            image[i][j].rgbtRed = average;             
            image[i][j].rgbtBlue = average; 
            image[i][j].rgbtGreen = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // loop rows
    for (int i = 0; i < height; i++)
    {
        // pointers at begining and end of each row
        RGBTRIPLE *ptr1 = &image[i][0];
        RGBTRIPLE *ptr2 = &image[i][width - 1];
       
        // loop through row
        while (ptr1 < ptr2)
        {
            // copy pixels value
            RGBTRIPLE temp = *ptr1;
            *ptr1 = *ptr2;
            *ptr2 = temp;
            
            // update pointers
            ptr1++;
            ptr2--;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // copy array
    RGBTRIPLE imageCopy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            imageCopy[i][j] = image[i][j];
        }
    }
    
    // loop throught array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // array to hold average RGB value
            unsigned int average [3] = {0, 0, 0};
            float counter = .0f;
            
            // loop throught (3x3) pixel box around i-th index 
            for (int x = i - 1; x <= i + 1; x++)
            {
                // corner and edge case condition
                if (x >= 0 && x < height)
                {
                    // loop throught (3x3) pixel box around j-th index 
                    for (int y = j - 1; y <= j + 1; y++)
                    {
                        // corner and edge case condition
                        if (y >= 0 && y < width)
                        {
                            // sum values in average array
                            average[0] += imageCopy[x][y].rgbtRed;
                            average[1] += imageCopy[x][y].rgbtGreen;
                            average[2] += imageCopy[x][y].rgbtBlue;
                            counter++;
                        }
                    }
                }
            }
            // update average values in image array
            image[i][j].rgbtRed = (int)(average[0] / counter + .5f);
            image[i][j].rgbtGreen = (int)(average[1] / counter + .5f);
            image[i][j].rgbtBlue = (int)(average[2] / counter + .5f);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // copy array
    RGBTRIPLE imageCopy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            imageCopy[i][j] = image[i][j];
        }
    }
    
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    
    // loop throught each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // arrays to sum RGB
            int sumGx[3] = {0, 0, 0};
            int sumGy[3] = {0, 0, 0};
            
            // loop 3x3 box around i-th
            for (int x = i - 1; x <= i + 1 ; x++)
            {
                // edge and corner condition, let through only valid values
                if (x >= 0 && x < height)
                {
                    // loop 3x3 box around j-th
                    for (int y = j - 1; y <= j + 1; y++)
                    {
                        // edge and corner condition ...
                        if (y >= 0 && y < width)
                        {
                            // compute Gx and Gy for red channel
                            sumGx[0] += imageCopy[x][y].rgbtRed * Gx[x - i + 1][y - j + 1];
                            sumGy[0] += imageCopy[x][y].rgbtRed * Gy[x - i + 1][y - j + 1];
                            
                            // compute Gx and Gy for green channel
                            sumGx[1] += imageCopy[x][y].rgbtGreen * Gx[x - i + 1][y - j + 1];
                            sumGy[1] += imageCopy[x][y].rgbtGreen * Gy[x - i + 1][y - j + 1];
                            
                            // // compute Gx and Gy for blue channel
                            sumGx[2] += imageCopy[x][y].rgbtBlue * Gx[x - i + 1][y - j + 1];
                            sumGy[2] += imageCopy[x][y].rgbtBlue * Gy[x - i + 1][y - j + 1];
                        }
                    }
                }
            }
            // new values for RGB
            int newRed = (int)(sqrt((double)(sumGx[0] * sumGx[0] + sumGy[0] * sumGy[0])) + 0.5);
            newRed > 255 ? (image[i][j].rgbtRed = 255) : (image[i][j].rgbtRed = newRed);
            
            int newGreen = (int)(sqrt((double)(sumGx[1] * sumGx[1] + sumGy[1] * sumGy[1])) + 0.5);
            newGreen > 255 ? (image[i][j].rgbtGreen = 255) : (image[i][j].rgbtGreen = newGreen);
            
            int newBlue = (int)(sqrt((double)(sumGx[2] * sumGx[2] + sumGy[2] * sumGy[2])) + 0.5);
            newBlue > 255 ? (image[i][j].rgbtBlue = 255) : (image[i][j].rgbtBlue = newBlue);
        }
    }
    return;
}
