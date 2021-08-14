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
#include "magnolia_common_data.h"
#include <vector>

class ImageProcessingOpenCv: public ImageProcessingMain
{
	public:
		virtual ~ImageProcessingOpenCv();
		virtual int image_processing_color_image_to_gray_image(magnolia_cmd_param_type *cmd, std::vector<pixbuf_label> &src_img, std::vector<pixbuf_label> &dst_img);
		virtual int image_processing_inversion(magnolia_cmd_param_type *cmd, std::vector<pixbuf_label> &src_img, std::vector<pixbuf_label> &dst_img);
		cv::Mat convert_gdk_pixbuf_to_cv_mat(Gdk::Pixbuf &src_img);
		virtual int image_processing_not_implemented(magnolia_cmd_param_type *cmd);

	protected:
	private:
};

typedef int (ImageProcessingOpenCv::*opencv_image_processing_handler)(magnolia_cmd_param_type*, std::vector<pixbuf_label>&, std::vector<pixbuf_label>&);

#endif //_IMAGE_PROCESSING_OPENCV_H 
