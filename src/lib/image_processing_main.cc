/*
   Copyright 2017 slowstarter80

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <iostream>
#include "image_processing_main.h"
#include "image_processing_magnolia.h"
#include "image_processing_opencv.h"

using namespace std;


extern "C" class ImageProcessingMain* lib_test(int a);

class ImageProcessingMain* lib_test(int a)
{
	cout<<"Hello world!"<<endl;
	cout<<"ImageProcessingMain lib_test! "<<a<< " "<<endl;

	if(a == 1)
	{
		ImageProcessingMagnolia *my_magnolia_lib;
		my_magnolia_lib = new ImageProcessingMagnolia;
		return my_magnolia_lib;
	}
	else if(a == 2)
	{
		ImageProcessingOpenCv *my_opencv_lib;
		my_opencv_lib = new ImageProcessingOpenCv;
		return my_opencv_lib; 
	}

	return NULL;
}

