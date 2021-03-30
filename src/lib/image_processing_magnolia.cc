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
	bool has_alpah_channel = src_img.get_has_alpha();
	std::cout<<"rowstride  = "<<src_row_stride<<endl;
	std::cout<<"iNChannels = "<<src_n_channels<<endl;
	std::cout<<"height = "<<height<<endl;
	std::cout<<"width = "<<width<<endl;
	std::cout<<"get_has_alpha() = "<<has_alpah_channel<<endl;

	for (int y = 0; y < height; y++) 
	{
		for (int x = 0; x < width; x++)
		{
			offset = y * src_img.get_rowstride() + x * src_n_channels;

			dst_pixels[offset] = 255 - src_pixels[offset];
			dst_pixels[offset + 1] = 255 - src_pixels[offset + 1];
			dst_pixels[offset + 2] = 255 - src_pixels[offset + 2];

			//No invert for alpha channel
			if (has_alpah_channel) {
				dst_pixels[offset + 3] = src_pixels[offset + 3];
			}

		}
	}

	return SUCCESS;
}

