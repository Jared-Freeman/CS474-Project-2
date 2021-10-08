
#include <array>
#include "image.h"

class ImageMask 
{
	public:
		//ctors
		ImageMask();
		ImageMask(int rows, int cols, int** intensity_values);
		ImageMask(ImageType& mask_image);
		
		//methods
		void ApplyMask(ImageType& source_image, ImageType& output_image);
		
	private:
		//members
		int** m_intensity_values;
		int m_rows;
		int m_cols;
		
		//private methods
		int CalculateWeightedSum
			(	ImageType& source_image
			, ImageType& output_image
			, int i
			, int j
			);
		
};
