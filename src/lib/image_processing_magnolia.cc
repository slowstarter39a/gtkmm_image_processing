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
#include "image_processing_magnolia.h"
#include "magnolia_logger.h"

using namespace std;
static const char *tag = __FILE__;

ImageProcessingMagnolia::~ImageProcessingMagnolia()
{
}

int ImageProcessingMagnolia::image_processing_inversion(magnolia_cmd_param_type *cmd, std::vector<pixbuf_label> &src_img, std::vector<pixbuf_label> &dst_img)
{
	Gdk::Pixbuf *src_img_pixbuf = src_img[0].pixbuf;
	Gdk::Pixbuf *dst_img_pixbuf = dst_img[0].pixbuf;
	*dst_img[0].text = "Inversion";

	if (src_img_pixbuf->get_colorspace() != Gdk::COLORSPACE_RGB) return MAGNOLIA_FAILURE;
	if (src_img_pixbuf->get_bits_per_sample() != 8) return MAGNOLIA_FAILURE;

	int width= src_img_pixbuf->get_width();
	int height= src_img_pixbuf->get_height();
	int offset = 0;
	guchar *src_pixels= src_img_pixbuf->get_pixels();
	guchar *dst_pixels= dst_img_pixbuf->get_pixels();

	int src_n_channels= src_img_pixbuf->get_n_channels();
	int src_row_stride = src_img_pixbuf->get_rowstride();
	bool has_alpha_channel = src_img_pixbuf->get_has_alpha();

	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "rowstride = %d\n", src_row_stride);
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "iNChannels= %d\n", src_n_channels);
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "height = %d\n", height);
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "width= %d\n", width);
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "get_has_alpha() = %d\n", has_alpha_channel);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			offset = y * src_img_pixbuf->get_rowstride() + x * src_n_channels;

			dst_pixels[offset] = 255 - src_pixels[offset];
			dst_pixels[offset + 1] = 255 - src_pixels[offset + 1];
			dst_pixels[offset + 2] = 255 - src_pixels[offset + 2];

			//No invert for alpha channel
			if (has_alpha_channel) {
				dst_pixels[offset + 3] = src_pixels[offset + 3];
			}
		}
	}

	return MAGNOLIA_SUCCESS;
}

int ImageProcessingMagnolia::image_processing_not_implemented(magnolia_cmd_param_type *cmd)
{
	MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "This image processing API has not been implemented! class = ImageProcessingMagnolia, cmd id = %d\n", cmd->mag_cmd);
	return MAGNOLIA_NOT_IMPLEMENTED;
}
