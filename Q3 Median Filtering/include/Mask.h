#include <array>
#include "image.h"
#include <cmath>
#include <vector>
#include <algorithm> // for sort

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

	void ApplyMedianFilter(ImageType& source_image, ImageType& output_image);
	void ApplyAverageFilter(ImageType& source_image, ImageType& output_image);

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
	int GetMedian(
		int N
		, int M
		, int i
		, int j
		, ImageType& ref_image);
	int GetAverage(
		int N
		, int M
		, int i
		, int j
		, ImageType& ref_image);
};
