 /*
  * @description: 
  * 
  *
  * @author: 
  * 
  *
  */
  
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <map>
#include <math.h>

#include <cstdlib>
#include <ctime>

#include "image.h"
#include "WriteImage.h"
#include "ReadImage.h"
#include "Mask.h"

#include "freeman_arg_parse.h" //A small utility I wrote for extracting command line args.

bool FLAG_DEBUG = true;

void PrintHistogram(std::map<int, float> hist);
void WriteImageToFile(std::string filename, ImageType& img);
void AddNoise(ImageType& image, int noise);	// noise should be 30 or 50 for 30% or 50% noise

int main(int argc, char** argv)
{
  //TODO: Print description of process to console (per program)

  //Extract args into vector
  std::vector<std::string> args;
  for(int i=1; i < argc; i++)
  {
    std::string next_element(argv[i]);  
    args.push_back(next_element);
  }
  
	//Fill data structures using args
  std::vector<std::string> imagePaths = ExtractArgs("-in", args);
  std::vector<std::string> outputPaths = ExtractArgs("-out", args);
  if(outputPaths.size() > 0 && outputPaths[0].length() > 0)
  {
    if(outputPaths[0][outputPaths[0].length()-1] != '/')
      {
        outputPaths[0] = outputPaths[0] + "/";
        //std::cout << outputPaths[0] << "\n";
      }
  }
  if(outputPaths.size() > 1)
  {
    std::cout << "Can only specify one output path. Output will be saved in the first output directory.\n";
  }  
    
  //Process each image
  for(int i=0; i < imagePaths.size(); i++)
  {
    std::cout << "_________________________\n";
    std::cout << "image " << i << ": \"" << imagePaths[i] << "\"\n";
    
    ImageType next_image;
    ImageType out_image;
    char *cstr = new char[imagePaths[i].length() + 1];
    strcpy(cstr, imagePaths[i].c_str());
    std::readImage(cstr, next_image);

    // DO STUFF
    // ...

    std::srand(std::time(nullptr)); // set seed for random number generator


    // Set up mask
    int sz = 3;
    int** arr = new int* [sz];
    for (int i = 0; i < sz; i++)
    {
        arr[i] = new int[sz];
        for (int j = 0; j < sz; j++)
        {
            arr[i][j] = 1;
        }
    }


	// Just a static 5x5 array for testing
    const int N = 5;
    const int M = 5;
    ImageType test(5, 5, 5);

    test.setPixelVal(0, 0, 52);
    test.setPixelVal(0, 1, 181);
    test.setPixelVal(0, 2, 229);
    test.setPixelVal(0, 3, 140);
    test.setPixelVal(0, 4, 170);
    test.setPixelVal(1, 0, 62);
    test.setPixelVal(1, 1, 153);
    test.setPixelVal(1, 2, 210);
    test.setPixelVal(1, 3, 89);
    test.setPixelVal(1, 4, 63);
    test.setPixelVal(2, 0, 52);
    test.setPixelVal(2, 1, 82);
    test.setPixelVal(2, 2, 95);
    test.setPixelVal(2, 3, 47);
    test.setPixelVal(2, 4, 151);
    test.setPixelVal(3, 0, 229);
    test.setPixelVal(3, 1, 246);
    test.setPixelVal(3, 2, 203);
    test.setPixelVal(3, 3, 134);
    test.setPixelVal(3, 4, 126);
    test.setPixelVal(4, 0, 218);
    test.setPixelVal(4, 1, 112);
    test.setPixelVal(4, 2, 131);
    test.setPixelVal(4, 3, 29);
    test.setPixelVal(4, 4, 53);

    ImageMask test_mask(sz, sz, arr);
    test_mask.ApplyLaplacian(test, out_image);

    out_image.scaleImageData();	// map any negative pixel values left over from mask application to the range [0, 255]

    std::cout << "Output modified, scaled: \n";
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; j++)
        {
            out_image.getPixelVal(i, j, q);
            std::cout << q << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";


    for (int i = 0; i < sz; i++)
        delete[] arr[i];
    delete[] arr;

   
    // END DO STUFF

    { //Write to file scope

      //determine original file name from path string
      std::string original_filename = "";
      for(int l = imagePaths[i].length(); imagePaths[i][l] != '/' && l >= 0; l--)
      {
        if(imagePaths[i][l] != '/')
        {
          //std::cout << imagePaths[i][l] << std::endl;

          std::string temp;
          temp += imagePaths[i][l];
          original_filename.insert(0, temp);
        }
        //throw out extension
        if(imagePaths[i][l] == '.')
        {
          original_filename = "";
        }
      }      


      std::string out_file = outputPaths[0] + original_filename + "_modified.pgm";
      WriteImageToFile(out_file, out_image);

    } //end Write to file

    delete [] cstr;

    std::cout << "\n";
  }
  
  return 0;
}

void AddNoise(ImageType& image, int noise)
{
    int row, col, Q, x; // x is a random value

    image.getImageInfo(row, col, Q);
    float percentNoise = float(noise) / 100.0; 
    int noisePixelCount = floor((row * col) * percentNoise);

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            x = std::rand() % 5;
            if (x == 0 && noisePixelCount > 0)
            {
                x = std::rand() % 2;
                if (x > 0)
                    x += 254;   // set x to 255
                image.setPixelVal(i, j, x); // set a random pixel to either black or white
                noisePixelCount--;
            }
        }
    }
}

void PrintHistogram(std::map<int, float> hist)
{
  for(auto it : hist)
  {
    std::cout << it.first << " " << it.second << std::endl;
  }
  std::cout << std::endl;
}

void WriteImageToFile(std::string filename, ImageType& img)
{
  std::string out_file = filename;
  char *cstr = new char[out_file.length() + 1];
  strcpy(cstr, out_file.c_str());

  std::writeImage(cstr, img);

  std::cout << " * Saved image: " << out_file << "\n";

  delete [] cstr;
}
