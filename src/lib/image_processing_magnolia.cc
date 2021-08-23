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
	Gdk::Pixbuf *src_pixbuf = src_img[0].pixbuf;
	struct gdk_pixbuf_image_info pixbuf_info;

	if (check_and_print_image_info(__FUNCTION__, src_pixbuf, &pixbuf_info) != MAGNOLIA_SUCCESS) {
		return MAGNOLIA_FAILURE;
	}

	int offset = 0;
	guchar *src_pixels= src_pixbuf->get_pixels();

	//lightness. (max(R, G, B) + min(R, G, B))/2
	Gdk::Pixbuf *dst_pixbuf = dst_img[0].pixbuf;
	guchar *dst_pixels= dst_pixbuf->get_pixels();
	*dst_img[0].text = "Color image to Gray image(lightness. (max(R,G,B)+min(R,G,B))/2)";

	for (int i = 0; i < pixbuf_info.height; i++) {
		for (int j = 0; j < pixbuf_info.width; j++) {
			offset = i * pixbuf_info.row_stride + j * pixbuf_info.n_channels;
			int lightness_value = (std::max(std::max(src_pixels[offset], src_pixels[offset + 1]), src_pixels[offset + 2]) +
								  std::min(std::min(src_pixels[offset], src_pixels[offset + 1]), src_pixels[offset + 2])) / 2;
			lightness_value = std::max(std::min(lightness_value, 255), 0);
			dst_pixels[offset] = dst_pixels[offset + 1] = dst_pixels[offset + 2] = lightness_value;

			if (pixbuf_info.has_alpha) {
				dst_pixels[offset + 3] = src_pixels[offset + 3];
			}
		}
	}

	//rgb average gray. (R + G + B) / 3
	dst_pixbuf = dst_img[1].pixbuf;
	dst_pixels= dst_pixbuf->get_pixels();
	*dst_img[1].text = "Color image to Gray image(rgb average gray. (R + G + B) / 3)";

	for (int i = 0; i < pixbuf_info.height; i++) {
		for (int j = 0; j < pixbuf_info.width; j++) {
			offset = i * pixbuf_info.row_stride + j * pixbuf_info.n_channels;
			int average_gray_value = (src_pixels[offset] + src_pixels[offset + 1] + src_pixels[offset + 2]) / 3;
			average_gray_value = std::max(std::min(average_gray_value, 255), 0);
			dst_pixels[offset] = dst_pixels[offset + 1] = dst_pixels[offset + 2] = average_gray_value;

			if (pixbuf_info.has_alpha) {
				dst_pixels[offset + 3] = src_pixels[offset + 3];
			}
		}
	}

	//luminosity. YCbCr Y = Red * 0.2126 + Geeen * 0.7152 + Blue * 0.0722
	dst_pixbuf = dst_img[2].pixbuf;
	dst_pixels= dst_pixbuf->get_pixels();
	*dst_img[2].text = "Color image to Gray image(luminosity. Y=Red*0.2126+Green*0.7152+Blue*0.0722)";

	for (int i = 0; i < pixbuf_info.height; i++) {
		for (int j = 0; j < pixbuf_info.width; j++) {
			offset = i * pixbuf_info.row_stride + j * pixbuf_info.n_channels;
			int luminosity_value = src_pixels[offset] * 0.2126 + src_pixels[offset + 1] * 0.7152 + src_pixels[offset + 2] * 0.0722;
			luminosity_value = std::max(std::min(luminosity_value, 255), 0);
			dst_pixels[offset] = dst_pixels[offset + 1] = dst_pixels[offset + 2] = luminosity_value;

			if (pixbuf_info.has_alpha) {
				dst_pixels[offset + 3] = src_pixels[offset + 3];
			}
		}
	}

	return MAGNOLIA_SUCCESS;
}

int ImageProcessingMagnolia::image_processing_inversion(magnolia_param_type *cmd, std::vector<pixbuf_label> &src_img, std::vector<pixbuf_label> &dst_img)
{
	Gdk::Pixbuf *src_pixbuf = src_img[0].pixbuf;
	Gdk::Pixbuf *dst_pixbuf = dst_img[0].pixbuf;
	*dst_img[0].text = "Inversion";
	struct gdk_pixbuf_image_info pixbuf_info;

	if (check_and_print_image_info(__FUNCTION__, src_pixbuf, &pixbuf_info) != MAGNOLIA_SUCCESS) {
		return MAGNOLIA_FAILURE;
	}

	int offset = 0;
	guchar *src_pixels= src_pixbuf->get_pixels();
	guchar *dst_pixels= dst_pixbuf->get_pixels();


	for (int i = 0; i < pixbuf_info.height; i++) {
		for (int j = 0; j < pixbuf_info.width; j++) {
			offset = i * pixbuf_info.row_stride + j * pixbuf_info.n_channels;

			dst_pixels[offset] = 255 - src_pixels[offset];
			dst_pixels[offset + 1] = 255 - src_pixels[offset + 1];
			dst_pixels[offset + 2] = 255 - src_pixels[offset + 2];

			//No invert for alpha channel
			if (pixbuf_info.has_alpha) {
				dst_pixels[offset + 3] = src_pixels[offset + 3];
			}
		}
	}

	return MAGNOLIA_SUCCESS;
}

int ImageProcessingMagnolia::image_processing_image_binarization(magnolia_param_type *cmd, std::vector<pixbuf_label> &src_img, std::vector<pixbuf_label> &dst_img)
{
	Gdk::Pixbuf *src_pixbuf = src_img[0].pixbuf;
	Gdk::Pixbuf *dst_pixbuf = dst_img[0].pixbuf;
	*dst_img[0].text = "Image binarization : threshold == " + to_string(cmd->param.img_binarization.value);
	struct gdk_pixbuf_image_info pixbuf_info;

	if (check_and_print_image_info(__FUNCTION__, src_pixbuf, &pixbuf_info) != MAGNOLIA_SUCCESS) {
		return MAGNOLIA_FAILURE;
	}

	int offset = 0;
	guchar *src_pixels= src_pixbuf->get_pixels();
	guchar *dst_pixels= dst_pixbuf->get_pixels();

	//luminosity. YCbCr Y = Red * 0.2126 + Geeen * 0.7152 + Blue * 0.0722
	dst_pixels= dst_pixbuf->get_pixels();

	for (int i = 0; i < pixbuf_info.height; i++) {
		for (int j = 0; j < pixbuf_info.width; j++) {
			offset = i * pixbuf_info.row_stride + j * pixbuf_info.n_channels;
			int luminosity_value = src_pixels[offset] * 0.2126 + src_pixels[offset + 1] * 0.7152 + src_pixels[offset + 2] * 0.0722;

			if (luminosity_value > cmd->param.img_binarization.value) {
				dst_pixels[offset] = dst_pixels[offset + 1] = dst_pixels[offset + 2] = 255;
			}
			else {
				dst_pixels[offset] = dst_pixels[offset + 1] = dst_pixels[offset + 2] = 0;
			}

			if (pixbuf_info.has_alpha) {
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
