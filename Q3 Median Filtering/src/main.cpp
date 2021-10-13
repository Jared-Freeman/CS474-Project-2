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
void AddNoise(ImageType& image, int noise);

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

    srand(time(0)); // seed for random numbers

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

    // ADD NOISE TO SOURCE IMAGE; 30% AND 50%
    ImageType noise30(next_image);
    ImageType noise50(next_image);

    AddNoise(noise30, 30);
    AddNoise(noise50, 50);

    WriteImageToFile(out_file + "_noise30.pgm", noise30);
    WriteImageToFile(out_file + "_noise50.pgm", noise50);

    // MASK GENERATION
    int sz = 7;
    int** arr = new int* [sz];
    for (int i = 0; i < sz; i++)
    {
        arr[i] = new int[sz];
        for (int j = 0; j < sz; j++)
        {
            arr[i][j] = 1;
        }
    }

    // APPLY MASKS - 7x7
    ImageMask mask_7(sz, sz, arr);

    mask_7.ApplyMedianFilter(noise30, out_image);
    WriteImageToFile(out_file + "_noise30_7x_clean.pgm", out_image);

    mask_7.ApplyMedianFilter(noise50, out_image);
    WriteImageToFile(out_file + "_noise50_7x_clean.pgm", out_image);

    for (int i = 0; i < sz; i++)
        delete[] arr[i];
    delete[] arr;


    // MASK GENERATION
    sz = 15;
    arr = new int* [sz];
    for (int i = 0; i < sz; i++)
    {
        arr[i] = new int[sz];
        for (int j = 0; j < sz; j++)
        {
            arr[i][j] = 1;
        }
    }

    ImageMask mask_15(sz, sz, arr);

    // APPLY MASKS - 15x15
    mask_15.ApplyMedianFilter(noise30, out_image);
    WriteImageToFile(out_file + "_noise30_15x_clean.pgm", out_image);

    mask_15.ApplyMedianFilter(noise50, out_image);
    WriteImageToFile(out_file + "_noise50_15x_clean.pgm", out_image);

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
