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
#include <opencv2/opencv.hpp>

class ImageProcessingOpenCv: public ImageProcessingMain
{
	public:
		virtual ~ImageProcessingOpenCv();
		virtual int image_processing_inversion(magnolia_cmd_param_type *cmd, Gdk::Pixbuf &src_img, Gdk::Pixbuf &dst_img);
		cv::Mat convert_gdk_pixbuf_to_cv_mat(Gdk::Pixbuf &src_img);
		virtual int image_processing_not_implemented(magnolia_cmd_param_type *cmd);

	protected:
	private:
};

typedef int (ImageProcessingOpenCv::*opencv_image_processing_handler)(magnolia_cmd_param_type*, Gdk::Pixbuf&, Gdk::Pixbuf&);

#endif //_IMAGE_PROCESSING_OPENCV_H 
