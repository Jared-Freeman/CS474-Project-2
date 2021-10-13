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
#include <math.h>

#include "image.h"
#include "WriteImage.h"
#include "ReadImage.h"
#include "Mask.h"

#include "freeman_arg_parse.h" //A small utility I wrote for extracting command line args.

bool FLAG_DEBUG = true;

void WriteImageToFile(std::string filename, ImageType& img);

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

    // GENERATE OUTPUT FILENAME CONVENTION
    //determine original file name from path string
    std::string original_filename = "";
    for (int l = imagePaths[i].length(); imagePaths[i][l] != '/' && l >= 0; l--)
    {
        if (imagePaths[i][l] != '/')
        {
            //std::cout << imagePaths[i][l] << std::endl;

            std::string temp;
            temp += imagePaths[i][l];
            original_filename.insert(0, temp);
        }
        //throw out extension
        if (imagePaths[i][l] == '.')
        {
            original_filename = "";
        }
    }
    std::string out_file = outputPaths[0] + original_filename;

    // MASK GENERATION
    int** arr;
    int sz;
    int arr_i;

    // Gaussian 7x7 mask
    sz = 7;
    arr = new int* [sz];
    arr_i = 0;

    arr[arr_i++] = new int[sz] {1, 1, 2, 2, 2, 1, 1};
    arr[arr_i++] = new int[sz] {1, 2, 2, 4, 2, 2, 1};
    arr[arr_i++] = new int[sz] {2, 2, 4, 8, 4, 2, 2};
    arr[arr_i++] = new int[sz] {2, 4, 8, 16, 8, 4, 2};
    arr[arr_i++] = new int[sz] {2, 2, 4, 8, 4, 2, 2};
    arr[arr_i++] = new int[sz] {1, 2, 2, 4, 2, 2, 1};
    arr[arr_i++] = new int[sz] {1, 1, 2, 2, 2, 1, 1};

    ImageMask mask_gaussian_7(sz, sz, arr);

    // This is unsharp filtering: k = 1
    mask_gaussian_7.ApplyHighBoostMask(next_image, out_image, 1);
    WriteImageToFile(out_file + "_unsharp.pgm", out_image);

    mask_gaussian_7.ApplyHighBoostMask(next_image, out_image, 2);
    WriteImageToFile(out_file + "_high2.pgm", out_image);

    mask_gaussian_7.ApplyHighBoostMask(next_image, out_image, 3);
    WriteImageToFile(out_file + "_high3.pgm", out_image);

    mask_gaussian_7.ApplyHighBoostMask(next_image, out_image, 4);
    WriteImageToFile(out_file + "_high4.pgm", out_image);

    for (int i = 0; i < sz; i++)
        delete[] arr[i];
    delete[] arr;


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

void WriteImageToFile(std::string filename, ImageType& img)
{
  std::string out_file = filename;
  char *cstr = new char[out_file.length() + 1];
  strcpy(cstr, out_file.c_str());

  std::writeImage(cstr, img);

  std::cout << " * Saved image: " << out_file << "\n";

  delete [] cstr;
}
