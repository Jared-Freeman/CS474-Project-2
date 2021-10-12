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

#include "image.h"
#include "WriteImage.h"
#include "ReadImage.h"
#include "Mask.h"

#include "freeman_arg_parse.h" //A small utility I wrote for extracting command line args.

bool FLAG_DEBUG = true;

void PrintHistogram(std::map<int, float> hist);
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
  if(outputPaths.size() < 1)
  {
    std::cout << "Please specify an output directory using the \"-out <output_path>\" args.\n";
    return 0;
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

    // GENERATE OUTPUT FILENAME CONVENTION
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
    std::string out_file = outputPaths[0] + original_filename;


    // MASK GENERATION

    int** arr;
    int sz;
    int arr_i;



    // Gaussian 15x15 mask
    sz = 15;
    arr = new int*[sz];
    arr_i = 0;
    
    arr[arr_i++] = new int[sz]  {2,2,3,4,5,5,6,6,6,5,5,4,3,2,2};
    arr[arr_i++] = new int[sz]  {2,3,4,5,7,7,8,8,8,7,7,5,4,3,2};
    arr[arr_i++] = new int[sz]  {3,4,6,7,9,10,10,11,10,10,9,7,6,4,3};
    arr[arr_i++] = new int[sz]  {4,5,7,9,10,12,13,13,13,12,10,9,7,5,4};
    arr[arr_i++] = new int[sz]  {5,7,9,11,13,14,15,16,15,14,13,11,9,7,5};
    arr[arr_i++] = new int[sz]  {5,7,10,12,14,16,17,18,17,16,14,12,10,7,5};
    arr[arr_i++] = new int[sz]  {6,8,10,13,15,17,19,19,19,17,15,13,10,8,6};
    arr[arr_i++] = new int[sz]  {6,8,11,13,16,18,19,20,19,18,16,13,11,8,6};
    arr[arr_i++] = new int[sz]  {6,8,10,13,15,17,19,19,19,17,15,13,10,8,6};
    arr[arr_i++] = new int[sz]  {5,7,10,12,14,16,17,18,17,16,14,12,10,7,5};
    arr[arr_i++] = new int[sz]  {5,7,9,11,13,14,15,16,15,14,13,11,9,7,5};
    arr[arr_i++] = new int[sz]  {4,5,7,9,10,12,13,13,13,12,10,9,7,5,4};
    arr[arr_i++] = new int[sz]  {3,4,6,7,9,10,10,11,10,10,9,7,6,4,3};
    arr[arr_i++] = new int[sz]  {2,3,4,5,7,7,8,8,8,7,7,5,4,3,2};
    arr[arr_i++] = new int[sz]  {2,2,3,4,5,5,6,6,6,5,5,4,3,2,2};

    ImageMask mask_gaussian_15(sz, sz, arr);
    mask_gaussian_15.ApplyMask(next_image, out_image);
    WriteImageToFile(out_file + "_gaussian_15x15.pgm", out_image);

    for(int i=0; i<sz; i++)
      delete [] arr[i];
    delete [] arr;



    // Gaussian 7x7 mask
    sz = 7;
    arr = new int*[sz];
    arr_i = 0;
    
    arr[arr_i++] = new int[sz]  {1,1,2,2,2,1,1};
    arr[arr_i++] = new int[sz]  {1,2,2,4,2,2,1};
    arr[arr_i++] = new int[sz]  {2,2,4,8,4,2,2};
    arr[arr_i++] = new int[sz]  {2,4,8,16,8,4,2};
    arr[arr_i++] = new int[sz]  {2,2,4,8,4,2,2};
    arr[arr_i++] = new int[sz]  {1,2,2,4,2,2,1};
    arr[arr_i++] = new int[sz]  {1,1,2,2,2,1,1};

    ImageMask mask_gaussian_7(sz, sz, arr);
    mask_gaussian_7.ApplyMask(next_image, out_image);
    WriteImageToFile(out_file + "_gaussian_7x7.pgm", out_image);

    for(int i=0; i<sz; i++)
      delete [] arr[i];
    delete [] arr;



    // Averaging 15x15 mask
    sz = 15;
    arr = new int*[sz];
    arr_i = 0;

    for(int i=0; i<sz; i++)
    {
      arr[i] = new int[sz];
      for(int j=0; j<sz; j++)
      {
        arr[i][j] = 1;
      }
    }

    ImageMask mask_average_15(sz, sz, arr);
    mask_average_15.ApplyMask(next_image, out_image);
    WriteImageToFile(out_file + "_average_15x15.pgm", out_image);

    for(int i=0; i<sz; i++)
      delete [] arr[i];
    delete [] arr;



    // Averaging 7x7 mask
    sz = 7;
    arr = new int*[sz];
    arr_i = 0;

    for(int i=0; i<sz; i++)
    {
      arr[i] = new int[sz];
      for(int j=0; j<sz; j++)
      {
        arr[i][j] = 1;
      }
    }

    ImageMask mask_average_7(sz, sz, arr);
    mask_average_7.ApplyMask(next_image, out_image);
    WriteImageToFile(out_file + "_average_7x7.pgm", out_image);

    for(int i=0; i<sz; i++)
      delete [] arr[i];
    delete [] arr;



    // END DO STUFF



    delete [] cstr;

    std::cout << "\n";
  }
  
  return 0;
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
