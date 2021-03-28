/*
 * =====================================================================================
 *
 *       Filename:  image_processing_magnolia.cc
 *
 *    Description:  
 *      This file is a image processing library source file
 *
 * =====================================================================================
 */
#include <iostream>
#include "image_processing_magnolia.h"

using namespace std;

ImageProcessingMagnolia::~ImageProcessingMagnolia()
{

}

int ImageProcessingMagnolia::image_processing_handler(magnolia_cmd_type *cmd, Gdk::Pixbuf &src_img, Gdk::Pixbuf &dst_img)
{
	if (src_img.get_colorspace() != Gdk::COLORSPACE_RGB) return FAILURE;
	if (src_img.get_bits_per_sample() != 8) return FAILURE;

	int width= src_img.get_width();
	int height= src_img.get_height();
	int offset = 0;
	guchar * src_pixels= src_img.get_pixels();
	guchar * dst_pixels= dst_img.get_pixels();

	int src_n_channels= src_img.get_n_channels(); 
	int src_row_stride = src_img.get_rowstride();
	std::cout<<"rowstride  = "<<src_row_stride<<endl;
	std::cout<<"iNChannels = "<<src_n_channels<<endl;
	std::cout<<"height = "<<height<<endl;
	std::cout<<"width = "<<width<<endl;

	for (int y = 0; y < height; y++) 
	{
		for (int x = 0; x < width; x++)
		{
			offset = y * src_img.get_rowstride() + x * src_n_channels;
			for (int channel = 0; channel < src_n_channels; channel++) {
				dst_pixels[offset + channel] = 255 - src_pixels[offset + channel];
			}
		}
	}

	return SUCCESS;
}

