/*
 * =====================================================================================
 *
 *       Filename:  image_processing_opencv.cc
 *
 *    Description:  
 *      This file is a image processing library source file which using opencv 
 *
 * =====================================================================================
 */

#include "image_processing_opencv.h"
#include "magnolia_logger.h"

using namespace std;
static const char *tag = __FILE__;


ImageProcessingOpenCv::~ImageProcessingOpenCv()
{
}

int ImageProcessingOpenCv::image_processing_inversion(magnolia_cmd_param_type *cmd, Gdk::Pixbuf &src_img, Gdk::Pixbuf &dst_img)
{
	MGNL_PRINTF(tag, LOG_LEVEL_TRACE, "opencv image_processing_handler()\n");
	if (src_img.get_colorspace() != Gdk::COLORSPACE_RGB ) return MAGNOLIA_FAILURE;
	if (src_img.get_bits_per_sample() != 8 ) return MAGNOLIA_FAILURE;
	cv::Mat opencv_src_img = convert_gdk_pixbuf_to_cv_mat(src_img);
	cv::Mat opencv_dst_img;

	bitwise_not(opencv_src_img, opencv_dst_img);
	int len = opencv_src_img.rows * opencv_src_img.cols * opencv_src_img.channels();

	//No invert for alpha channel
	if (src_img.get_has_alpha()) {
		for (int i = 3; i < len; i+=4) {
			opencv_dst_img.data[i] = ~opencv_dst_img.data[i];
		}
	}

	guchar * dst_pixels= dst_img.get_pixels();
	memcpy(dst_pixels, opencv_dst_img.data, opencv_src_img.rows * opencv_src_img.cols * opencv_src_img.channels());

	return MAGNOLIA_SUCCESS;
}

cv::Mat ImageProcessingOpenCv::convert_gdk_pixbuf_to_cv_mat(Gdk::Pixbuf &src_img)
{
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "height = %d\n", src_img.get_height());
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "width= %d\n", src_img.get_width());
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "get_has_alpha() = %d\n", src_img.get_has_alpha());
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "iNChannels= %d\n", src_img.get_n_channels());

	if (src_img.get_has_alpha()) {
		cv::Mat opencv_src_img(cv::Size(src_img.get_width(), src_img.get_height()), CV_8UC4, (uchar*)src_img.get_pixels(), src_img.get_rowstride());
		return opencv_src_img;
	}
	else {
		cv::Mat opencv_src_img(cv::Size(src_img.get_width(), src_img.get_height()), CV_8UC3, (uchar*)src_img.get_pixels(), src_img.get_rowstride());
		return opencv_src_img;
	}
}

int ImageProcessingOpenCv::image_processing_not_implemented(magnolia_cmd_param_type *cmd)
{
	MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "This image processing API has not been implemented! class = ImageProcessingOpenCv, cmd id = %d\n", cmd->mag_cmd);
	return MAGNOLIA_NOT_IMPLEMENTED;
}
