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

int ImageProcessingMagnolia::image_processing_color_image_to_gray_image(magnolia_param_type *cmd, std::vector<pixbuf_label> &src_img, std::vector<pixbuf_label> &dst_img)
{
	Gdk::Pixbuf *src_img_pixbuf = src_img[0].pixbuf;

	if (src_img_pixbuf->get_colorspace() != Gdk::COLORSPACE_RGB) return MAGNOLIA_FAILURE;
	if (src_img_pixbuf->get_bits_per_sample() != 8) return MAGNOLIA_FAILURE;

	int width= src_img_pixbuf->get_width();
	int height= src_img_pixbuf->get_height();
	int offset = 0;
	guchar *src_pixels= src_img_pixbuf->get_pixels();

	int src_n_channel = src_img_pixbuf->get_n_channels();
	int src_row_stride = src_img_pixbuf->get_rowstride();
	bool has_alpha_channel = src_img_pixbuf->get_has_alpha();

	print_source_image_info(__FUNCTION__, src_row_stride, src_n_channel, has_alpha_channel, width, height);

	//lightness. (max(R, G, B) + min(R, G, B))/2
	Gdk::Pixbuf *dst_img_pixbuf = dst_img[0].pixbuf;
	guchar *dst_pixels= dst_img_pixbuf->get_pixels();
	*dst_img[0].text = "Color image to Gray image(lightness. (max(R,G,B)+min(R,G,B))/2)";

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			offset = y * src_img_pixbuf->get_rowstride() + x * src_n_channel;
			int lightness_value = (std::max(std::max(src_pixels[offset], src_pixels[offset + 1]), src_pixels[offset + 2]) +
								  std::min(std::min(src_pixels[offset], src_pixels[offset + 1]), src_pixels[offset + 2])) / 2;

			dst_pixels[offset] = dst_pixels[offset + 1] = dst_pixels[offset + 2] = lightness_value;

			if (has_alpha_channel) {
				dst_pixels[offset + 3] = src_pixels[offset + 3];
			}
		}
	}

	//rgb average gray. (R + G + B) / 3
	dst_img_pixbuf = dst_img[1].pixbuf;
	dst_pixels= dst_img_pixbuf->get_pixels();
	*dst_img[1].text = "Color image to Gray image(rgb average gray. (R + G + B) / 3)";

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			offset = y * src_img_pixbuf->get_rowstride() + x * src_n_channel;
			int average_gray_value = (src_pixels[offset] + src_pixels[offset + 1] + src_pixels[offset + 2]) / 3;

			dst_pixels[offset] = dst_pixels[offset + 1] = dst_pixels[offset + 2] = average_gray_value;

			if (has_alpha_channel) {
				dst_pixels[offset + 3] = src_pixels[offset + 3];
			}
		}
	}

	//luminosity. YCbCr Y = Red * 0.2126 + Geeen * 0.7152 + Blue * 0.0722
	dst_img_pixbuf = dst_img[2].pixbuf;
	dst_pixels= dst_img_pixbuf->get_pixels();
	*dst_img[2].text = "Color image to Gray image(luminosity. Y=Red*0.2126+Green*0.7152+Blue*0.0722)";

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			offset = y * src_img_pixbuf->get_rowstride() + x * src_n_channel;
			int luminosity_value = src_pixels[offset] * 0.2126 + src_pixels[offset + 1] * 0.7152 + src_pixels[offset + 2] * 0.0722;

			dst_pixels[offset] = dst_pixels[offset + 1] = dst_pixels[offset + 2] = luminosity_value;

			if (has_alpha_channel) {
				dst_pixels[offset + 3] = src_pixels[offset + 3];
			}
		}
	}

	return MAGNOLIA_SUCCESS;
}

int ImageProcessingMagnolia::image_processing_inversion(magnolia_param_type *cmd, std::vector<pixbuf_label> &src_img, std::vector<pixbuf_label> &dst_img)
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

	int src_n_channel = src_img_pixbuf->get_n_channels();
	int src_row_stride = src_img_pixbuf->get_rowstride();
	bool has_alpha_channel = src_img_pixbuf->get_has_alpha();

	print_source_image_info(__FUNCTION__, src_row_stride, src_n_channel, has_alpha_channel, width, height);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			offset = y * src_img_pixbuf->get_rowstride() + x * src_n_channel;

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

int ImageProcessingMagnolia::image_processing_image_binarization(magnolia_param_type *cmd, std::vector<pixbuf_label> &src_img, std::vector<pixbuf_label> &dst_img)
{
	Gdk::Pixbuf *src_img_pixbuf = src_img[0].pixbuf;
	Gdk::Pixbuf *dst_img_pixbuf = dst_img[0].pixbuf;
	*dst_img[0].text = "Image binarization : threshold == " + to_string(cmd->param.img_binarization.value);

	if (src_img_pixbuf->get_colorspace() != Gdk::COLORSPACE_RGB) return MAGNOLIA_FAILURE;
	if (src_img_pixbuf->get_bits_per_sample() != 8) return MAGNOLIA_FAILURE;

	int width= src_img_pixbuf->get_width();
	int height= src_img_pixbuf->get_height();
	int offset = 0;
	guchar *src_pixels= src_img_pixbuf->get_pixels();
	guchar *dst_pixels= dst_img_pixbuf->get_pixels();

	int src_n_channel = src_img_pixbuf->get_n_channels();
	int src_row_stride = src_img_pixbuf->get_rowstride();
	bool has_alpha_channel = src_img_pixbuf->get_has_alpha();

	print_source_image_info(__FUNCTION__, src_row_stride, src_n_channel, has_alpha_channel, width, height);

	//luminosity. YCbCr Y = Red * 0.2126 + Geeen * 0.7152 + Blue * 0.0722
	dst_pixels= dst_img_pixbuf->get_pixels();

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			offset = y * src_img_pixbuf->get_rowstride() + x * src_n_channel;
			int luminosity_value = src_pixels[offset] * 0.2126 + src_pixels[offset + 1] * 0.7152 + src_pixels[offset + 2] * 0.0722;

			if (luminosity_value < cmd->param.img_binarization.value) {
				dst_pixels[offset] = dst_pixels[offset + 1] = dst_pixels[offset + 2] = 255;
			}
			else {
				dst_pixels[offset] = dst_pixels[offset + 1] = dst_pixels[offset + 2] = 0;
			}

			if (has_alpha_channel) {
				dst_pixels[offset + 3] = src_pixels[offset + 3];
			}
		}
	}

	return MAGNOLIA_SUCCESS;
}

int ImageProcessingMagnolia::image_processing_not_implemented(magnolia_param_type *cmd)
{
	MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "This image processing API has not been implemented! class = ImageProcessingMagnolia, cmd id = %d\n", cmd->mag_cmd);
	return MAGNOLIA_NOT_IMPLEMENTED;
}
