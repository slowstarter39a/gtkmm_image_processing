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

int ImageProcessingOpenCv::image_processing_color_image_to_gray_image(magnolia_param_type *cmd, std::vector<pixbuf_label> &src_img, std::vector<pixbuf_label> &dst_img)
{
	Gdk::Pixbuf *src_img_pixbuf = src_img[0].pixbuf;
	Gdk::Pixbuf *dst_img_pixbuf = dst_img[0].pixbuf;
	*dst_img[0].text = "Color image to Gray image(rgb average gray)";

	print_source_image_info(__FUNCTION__, (*src_img_pixbuf).get_rowstride(), (*src_img_pixbuf).get_n_channels(),
			(*src_img_pixbuf).get_has_alpha(), (*src_img_pixbuf).get_width(), (*src_img_pixbuf).get_height());

	if (src_img_pixbuf->get_colorspace() != Gdk::COLORSPACE_RGB ) return MAGNOLIA_FAILURE;
	if (src_img_pixbuf->get_bits_per_sample() != 8 ) return MAGNOLIA_FAILURE;
	cv::Mat opencv_src_img = convert_gdk_pixbuf_to_cv_mat(*src_img_pixbuf);
	cv::Mat opencv_dst_img = cv::Mat::zeros(opencv_src_img.rows, opencv_src_img.cols, opencv_src_img.type());

	for (int i = 0; i < opencv_src_img.rows; i++) {
		for (int j = 0; j < opencv_src_img.cols; j++) {
			cv::Vec3b color = opencv_src_img.at<cv::Vec3b>(cv::Point(i,j));
			color.val[0]= (color.val[0]+color.val[1]+color.val[2])/3;
			color.val[1] = color.val[2] = color.val[0];

			opencv_dst_img.at<cv::Vec3b>(cv::Point(i,j)) = color;
		}
	}

	guchar *dst_pixels= dst_img_pixbuf->get_pixels();
	memcpy(dst_pixels, opencv_dst_img.data, opencv_src_img.rows * opencv_src_img.cols * opencv_src_img.channels());

	return MAGNOLIA_SUCCESS;
}

int ImageProcessingOpenCv::image_processing_inversion(magnolia_param_type *cmd, std::vector<pixbuf_label> &src_img, std::vector<pixbuf_label> &dst_img)
{
	Gdk::Pixbuf *src_img_pixbuf = src_img[0].pixbuf;
	Gdk::Pixbuf *dst_img_pixbuf = dst_img[0].pixbuf;
	*dst_img[0].text = "Inversion";

	print_source_image_info(__FUNCTION__, (*src_img_pixbuf).get_rowstride(), (*src_img_pixbuf).get_n_channels(),
			(*src_img_pixbuf).get_has_alpha(), (*src_img_pixbuf).get_width(), (*src_img_pixbuf).get_height());

	if (src_img_pixbuf->get_colorspace() != Gdk::COLORSPACE_RGB ) return MAGNOLIA_FAILURE;
	if (src_img_pixbuf->get_bits_per_sample() != 8 ) return MAGNOLIA_FAILURE;
	cv::Mat opencv_src_img = convert_gdk_pixbuf_to_cv_mat(*src_img_pixbuf);
	cv::Mat opencv_dst_img;

	bitwise_not(opencv_src_img, opencv_dst_img);
	int len = opencv_src_img.rows * opencv_src_img.cols * opencv_src_img.channels();

	//No invert for alpha channel
	if (src_img_pixbuf->get_has_alpha()) {
		for (int i = 3; i < len; i+=4) {
			opencv_dst_img.data[i] = ~opencv_dst_img.data[i];
		}
	}

	guchar *dst_pixels= dst_img_pixbuf->get_pixels();
	memcpy(dst_pixels, opencv_dst_img.data, opencv_src_img.rows * opencv_src_img.cols * opencv_src_img.channels());

	return MAGNOLIA_SUCCESS;
}

int ImageProcessingOpenCv::image_processing_image_binarization(magnolia_param_type *cmd, std::vector<pixbuf_label> &src_img, std::vector<pixbuf_label> &dst_img)
{
	return MAGNOLIA_SUCCESS;
}

cv::Mat ImageProcessingOpenCv::convert_gdk_pixbuf_to_cv_mat(Gdk::Pixbuf &src_img)
{
	if (src_img.get_has_alpha()) {
		cv::Mat opencv_src_img(cv::Size(src_img.get_width(), src_img.get_height()), CV_8UC4, (uchar*)src_img.get_pixels(), src_img.get_rowstride());
		return opencv_src_img;
	}
	else {
		cv::Mat opencv_src_img(cv::Size(src_img.get_width(), src_img.get_height()), CV_8UC3, (uchar*)src_img.get_pixels(), src_img.get_rowstride());
		return opencv_src_img;
	}
}

int ImageProcessingOpenCv::image_processing_not_implemented(magnolia_param_type *cmd)
{
	MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "This image processing API has not been implemented! class = ImageProcessingOpenCv, cmd id = %d\n", cmd->mag_cmd);
	return MAGNOLIA_NOT_IMPLEMENTED;
}
