#include <stdlib.h>

#include "image.h"

ImageType::ImageType()
{
    N = 0;
    M = 0;
    Q = 0;

    pixelValue = NULL;
}

ImageType::ImageType(int tmpN, int tmpM, int tmpQ)
{
    int i, j;

    N = tmpN;
    M = tmpM;
    Q = tmpQ;

    pixelValue = new int* [N];
    for (i = 0; i < N; i++) {
        pixelValue[i] = new int[M];
        for (j = 0; j < M; j++)
            pixelValue[i][j] = 0;
    }
}

ImageType::ImageType(ImageType& oldImage)
{
    int i, j;

    N = oldImage.N;
    M = oldImage.M;
    Q = oldImage.Q;

    pixelValue = new int* [N];
    for (i = 0; i < N; i++) {
        pixelValue[i] = new int[M];
        for (j = 0; j < M; j++)
            pixelValue[i][j] = oldImage.pixelValue[i][j];
    }
}

ImageType::~ImageType()
{
    int i;

    for (i = 0; i < N; i++)
        delete[] pixelValue[i];
    delete[] pixelValue;
}


void ImageType::getImageInfo(int& rows, int& cols, int& levels)
{
    rows = N;
    cols = M;
    levels = Q;
}

void ImageType::setImageInfo(int rows, int cols, int levels)
{
    N = rows;
    M = cols;
    Q = levels;
}

void ImageType::setPixelVal(int i, int j, int val)
{
    pixelValue[i][j] = val;
}

void ImageType::getPixelVal(int i, int j, int& val)
{
    val = pixelValue[i][j];
}



void ImageType::CombineImages(ImageType& add, int factor)
{   // eq 3-63 in book
    int T, U, R;
    add.getImageInfo(T, U, R);

    if (N != T || M != U)
    {
        std::cout << "Cannot combine images, dimensions do not match\n";
        return;
    }

    // Add the pixel values of 'add' image to this image's pixels and scale by the factor
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            add.getPixelVal(i, j, R);
            getPixelVal(i, j, Q);
            setPixelVal(i, j, (factor * (Q + R)));
        }
    }
}



void ImageType::scaleImageData()
{
    int min = 255;
    int K = 255;    // for 8-bit images
    
    // find min of image
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            getPixelVal(i, j, Q);
            if (Q < min)
                min = Q;
        }
    }

    // first operation: create image with min. value of 0
    // eq 2-31
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            getPixelVal(i, j, Q);
            setPixelVal(i, j, (Q - min));
        }
    }

    int max = 0;
    // Find max of image_min
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            getPixelVal(i, j, Q);
            if (Q > max)
                max = Q;
        }
    }
    std::cout << "Min: " << min << "\n";
    std::cout << "Max: " << max << "\n";
    // second operation: scale everything to [0, 255]
    //eq 2-32
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            getPixelVal(i, j, Q);
            setPixelVal(i, j, (int(K * (float(Q) / max))));
        }
    }
}

