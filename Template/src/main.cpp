 /*
  * @description: 
  * 
  *
  * @author: 
  * Jared Freeman
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

#include "freeman_arg_parse.h" //A small utility I wrote for extracting command line args.

bool FLAG_DEBUG = true;

void PrintHistogram(std::map<int, float> hist);

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
    char *cstr = new char[imagePaths[i].length() + 1];
    strcpy(cstr, imagePaths[i].c_str());
    std::readImage(cstr, next_image);

    // DO STUFF
    // ...
    // END DO SUFF

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

      // TODO: Change naming convention 
      std::string out_file = outputPaths[0] + original_filename + "_specified_" 
        + original_filename_s + ".pgm";
      char *cstr = new char[out_file.length() + 1];
      strcpy(cstr, out_file.c_str());

      std::writeImage(cstr, image_copy);

      std::cout << " * Saved image: " << out_file << "\n";
      delete [] cstr;
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
