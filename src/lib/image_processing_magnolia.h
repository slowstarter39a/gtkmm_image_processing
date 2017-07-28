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

class ImageProcessingMagnolia : public ImageProcessingMain
{
	public:
		virtual ~ImageProcessingMagnolia();
 		virtual int image_processing_handler(magnolia_cmd_type *cmd, Gdk::Pixbuf &src_img, Gdk::Pixbuf &dst_img);
	protected:
	private:


};


#endif //_IMAGE_PROCESSING_MAGNOLIA_H 
