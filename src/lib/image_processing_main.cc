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
#include "magnolia_logger.h"

using namespace std;
static const char *tag = __FILE__;


extern "C" int ImageProcessingDispatcher(int lib_type, magnolia_cmd_type *cmd, Gdk::Pixbuf &src_img, Gdk::Pixbuf &dst_img);

int ImageProcessingDispatcher(int lib_type, magnolia_cmd_type *cmd, Gdk::Pixbuf &src_img, Gdk::Pixbuf &dst_img)
{
	int result = FAILURE;
	MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "Hello world!\n");
	MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "ImageProcessingMain ImageProcessingDispatcher! lib_type = %d\n", lib_type);

	if(lib_type == 0)
	{
		ImageProcessingMagnolia *magnolia_processing;
		magnolia_processing= new ImageProcessingMagnolia;

		result  = magnolia_processing->image_processing_handler(cmd, src_img, dst_img); 

		delete magnolia_processing;
		return result;
	}
	else if(lib_type == 1)
	{
		ImageProcessingOpenCv *opencv_processing;
		opencv_processing= new ImageProcessingOpenCv;

		result = opencv_processing->image_processing_handler(cmd, src_img, dst_img);

		delete opencv_processing; 
		return result; 
	}

	return FAILURE;
}


ImageProcessingMain::~ImageProcessingMain()
{

}
