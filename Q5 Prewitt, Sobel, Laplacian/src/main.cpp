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
    ImageType sharp_image;
    char *cstr = new char[imagePaths[i].length() + 1];
    strcpy(cstr, imagePaths[i].c_str());
    std::readImage(cstr, next_image);

    // DO STUFF
     // ...

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

    /*
    // Just a static 5x5 array for testing
    const int N = 6;
    const int M = 6;
    ImageType test(6, 6, 2);

    test.setPixelVal(0, 0, 5);
    test.setPixelVal(0, 1, 5);
    test.setPixelVal(0, 2, 5);
    test.setPixelVal(0, 3, 5);
    test.setPixelVal(0, 4, 5);
    test.setPixelVal(0, 5, 5);
    test.setPixelVal(1, 0, 5);
    test.setPixelVal(1, 1, 5);
    test.setPixelVal(1, 2, 5);
    test.setPixelVal(1, 3, 5);
    test.setPixelVal(1, 4, 5);
    test.setPixelVal(1, 5, 5);
    test.setPixelVal(2, 0, 5);
    test.setPixelVal(2, 1, 5);
    test.setPixelVal(2, 2, 10);
    test.setPixelVal(2, 3, 10);
    test.setPixelVal(2, 4, 10);
    test.setPixelVal(2, 5, 10);
    test.setPixelVal(3, 0, 5);
    test.setPixelVal(3, 1, 5);
    test.setPixelVal(3, 2, 10);
    test.setPixelVal(3, 3, 10);
    test.setPixelVal(3, 4, 10);
    test.setPixelVal(3, 5, 10);
    test.setPixelVal(4, 0, 5);
    test.setPixelVal(4, 1, 5);
    test.setPixelVal(4, 2, 5);
    test.setPixelVal(4, 3, 10);
    test.setPixelVal(4, 4, 10);
    test.setPixelVal(4, 5, 10);
    test.setPixelVal(5, 0, 5);
    test.setPixelVal(5, 1, 5);
    test.setPixelVal(5, 2, 5);
    test.setPixelVal(5, 3, 5);
    test.setPixelVal(5, 4, 10);
    test.setPixelVal(5, 5, 10);
    int q;

    std::cout << "test: \n";
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; j++)
        {
            test.getPixelVal(i, j, q);
            std::cout << q << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";

    */


    // sharp_image.CopyImageData(next_image);
    // sharp_image.CombineImages(out_image, 1, true);
    // sharp_image.CombineImages(out_image, 1, true);
    // WriteImageToFile(out_file + "_PrewittSharp.pgm", sharp_image);

    ImageMask test_mask(sz, sz, arr);

    //test_mask.ApplyLaplacian(next_image, out_image);


    // 1ST DERIVATIVE - PREWITT AND SOBEL
    test_mask.ApplyPrewittX(next_image, out_image);

    out_image.RemapPixelValues();

    WriteImageToFile(out_file + "_PrewittX.pgm", out_image);

    
    test_mask.ApplyPrewittY(next_image, out_image);
    out_image.RemapPixelValues();
    WriteImageToFile(out_file + "_PrewittY.pgm", out_image);


    test_mask.ApplyPrewittMag(next_image, out_image);
    //out_image.scaleImageData();
    WriteImageToFile(out_file + "_PrewittMag.pgm", out_image);

    out_image.CombineImages(next_image, 1);
    WriteImageToFile(out_file + "_PrewittSharp.pgm", out_image);




    test_mask.ApplySobelX(next_image, out_image);
    out_image.RemapPixelValues();
    WriteImageToFile(out_file + "_SobelX.pgm", out_image);

    test_mask.ApplySobelY(next_image, out_image);
    out_image.RemapPixelValues();
    WriteImageToFile(out_file + "_SobelY.pgm", out_image);

    test_mask.ApplySobelMag(next_image, out_image);

    //out_image.scaleImageData();
    WriteImageToFile(out_file + "_SobelMag.pgm", out_image);

    out_image.CombineImages(next_image, 1);
    WriteImageToFile(out_file + "_SobelSharp.pgm", out_image);

    /*
    int N, M, q;
    out_image.getImageInfo(N, M, q);
    std::cout << "Output, ApplyPrewittX: \n";
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; j++)
        {
            out_image.getPixelVal(i, j, q);
            std::cout << q << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";
    */

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


      // std::string out_file = outputPaths[0] + original_filename + "_modified.pgm";
      // WriteImageToFile(out_file, out_image);

    } //end Write to file

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
