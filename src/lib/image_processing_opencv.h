/*
 * =====================================================================================
 *
 *       Filename:  image_processing_opencv.h
 *
 *    Description:  
 *      This file is a image processing library header file which using opencv 
 *
 * =====================================================================================
 */

#ifndef _IMAGE_PROCESSING_OPENCV_H
#define _IMAGE_PROCESSING_OPENCV_H 

#include <gtkmm.h>
#include "image_processing_main.h"

class ImageProcessingOpenCv: public ImageProcessingMain
{
	public:
		virtual int lib_func();
	protected:
	private:


};

#endif //_IMAGE_PROCESSING_OPENCV_H 
