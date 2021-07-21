/*
 * =====================================================================================
 *
 *       Filename:  image_processing_opencv.cc
 *
 *    Description:  
 *      This file is a image processing library source file which using opencv 
 *
 * =====================================================================================
 */

#include <iostream>
#include "image_processing_opencv.h"
#include <opencv2/opencv.hpp>
#include "magnolia_logger.h"

using namespace std;
static const char *tag = __FILE__;


ImageProcessingOpenCv::~ImageProcessingOpenCv()
{

}

int ImageProcessingOpenCv::image_processing_handler(magnolia_cmd_type *cmd, Gdk::Pixbuf &src_img, Gdk::Pixbuf &dst_img)
{
	MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "opencv image_processing_handler()\n");
	if (src_img.get_colorspace() != Gdk::COLORSPACE_RGB ) return FAILURE;
	if (src_img.get_bits_per_sample() != 8 ) return FAILURE;

	cv::Mat opencv_src_img(cv::Size(src_img.get_width(), src_img.get_height()), CV_8UC3, (uchar*)src_img.get_pixels(), src_img.get_rowstride());
	int offset = 0;
	int channel = opencv_src_img.channels();

	guchar * dst_pixels= dst_img.get_pixels();
	for (int y = 0; y < opencv_src_img.rows; y++) 
	{
		for (int x= 0; x < opencv_src_img.cols; x++) 
		{
			offset = y * opencv_src_img.step.buf[0];
			for(int ch = 0; ch < channel; ch++)
			{
				dst_pixels[offset + channel * x + ch] = 255 - opencv_src_img.data[offset + channel * x + ch]; 
			}
		}
	}

	return SUCCESS;
}
