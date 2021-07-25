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
#include "magnolia_logger.h"

using namespace std;
static const char *tag = __FILE__;

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
	bool has_alpha_channel = src_img.get_has_alpha();

	MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "rowstride = %d\n", src_row_stride);
	MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "iNChannels= %d\n", src_n_channels);
	MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "height = %d\n", height);
	MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "width= %d\n", width);
	MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "get_has_alpha() = %d\n", has_alpha_channel);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			offset = y * src_img.get_rowstride() + x * src_n_channels;

			dst_pixels[offset] = 255 - src_pixels[offset];
			dst_pixels[offset + 1] = 255 - src_pixels[offset + 1];
			dst_pixels[offset + 2] = 255 - src_pixels[offset + 2];

			//No invert for alpha channel
			if (has_alpha_channel) {
				dst_pixels[offset + 3] = src_pixels[offset + 3];
			}
		}
	}

	return SUCCESS;
}

