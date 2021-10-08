
#include "Mask.h"

ImageMask::ImageMask():
    m_rows(0)
,   m_cols(0)
,   m_intensity_values(nullptr)
{}

ImageMask::ImageMask(int rows, int cols, int** intensity_values):
    m_rows(rows)
,   m_cols(cols)
{
    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<cols; j++)
        {
            
        }
    }
}

ImageMask(ImageType& mask_image)
{
    int Q;
    mask_image.getImageInfo(m_rows, m_cols, Q);

    int val = 0;
    for(int i=0; i<m_rows; i++)
    {
        for(int j=0; j<m_cols; j++)
        {
            mask_image.getPixelVal(i, j, val);
            intensity_values[i][j] = val;
        }
    }
}

ImageMask::~ImageMask()
{
    if(m_intensity_values != nullptr)
    {
        int i;
        for(i=0; i<rows; i++)
            delete [] m_intensity_values[i];
        delete [] m_intensity_values;
    }
}