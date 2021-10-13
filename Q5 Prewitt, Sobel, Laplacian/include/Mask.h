#include <array>
#include "image.h"
#include <cmath>

class ImageMask
{
public:
	//ctors
	ImageMask();
	ImageMask(int rows, int cols, int** intensity_values);
	ImageMask(int rows, int cols, float** intensity_values);
	ImageMask(ImageType& mask_image);

	//dtor
	~ImageMask();

	//methods
	void ApplyMask(ImageType& source_image, ImageType& output_image
		, bool flag_normalize = true);

	void ApplyLaplacian(ImageType& source_image, ImageType& output_image);

	void ApplyPrewittX(ImageType& source_image, ImageType& output_image);
	void ApplyPrewittY(ImageType& source_image, ImageType& output_image);
	void ApplyPrewittMag(ImageType& source_image, ImageType& output_image);

	void ApplySobelX(ImageType& source_image, ImageType& output_image);
	void ApplySobelY(ImageType& source_image, ImageType& output_image);
	void ApplySobelMag(ImageType& source_image, ImageType& output_image);

private:
	//members
	float** m_intensity_values;
	int m_rows;
	int m_cols;

	//private methods
	void AllocateMemory();
	void ClampIndex(int& val, int lo, int hi);
	int GetWeightedSum(
		int N
		, int M
		, int i
		, int j
		, ImageType& ref_image
		, bool flag_normalize = true
	);
};
