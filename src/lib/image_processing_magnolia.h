/*
 * =====================================================================================
 *
 *       Filename:  image_processing_magnolia.cc
 *
 *    Description:  
 *      This file is a image processing library header file
 *
 * =====================================================================================
 */
#ifndef _IMAGE_PROCESSING_MAGNOLIA_H
#define _IMAGE_PROCESSING_MAGNOLIA_H 

#include <gtkmm.h>
#include "image_processing_main.h"
#include "magnolia_common_data.h"
#include <vector>

class ImageProcessingMagnolia : public ImageProcessingMain
{
	public:
		virtual ~ImageProcessingMagnolia();
		virtual int image_processing_inversion(magnolia_cmd_param_type *cmd, std::vector<pixbuf_label> &src_img, std::vector<pixbuf_label> &dst_img);
		virtual int image_processing_not_implemented(magnolia_cmd_param_type *cmd);

	protected:
	private:
};

typedef int (ImageProcessingMagnolia::*magnolia_image_processing_handler)(magnolia_cmd_param_type*, std::vector<pixbuf_label>&, std::vector<pixbuf_label>&);

#endif //_IMAGE_PROCESSING_MAGNOLIA_H 
