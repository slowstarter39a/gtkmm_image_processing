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
	Gdk::Pixbuf *src_pixbuf = src_img[0].pixbuf;
	struct gdk_pixbuf_image_info pixbuf_info;

	if (check_and_print_image_info(__FUNCTION__, src_pixbuf, &pixbuf_info) != MAGNOLIA_SUCCESS) {
		return MAGNOLIA_FAILURE;
	}

	cv::Mat opencv_src_img = convert_gdk_pixbuf_to_cv_mat(*src_pixbuf);
	cv::Mat opencv_dst_img = cv::Mat::zeros(opencv_src_img.rows, opencv_src_img.cols, opencv_src_img.type());
	//lightness. (max(R, G, B) + min(R, G, B))/2
	Gdk::Pixbuf *dst_pixbuf = dst_img[0].pixbuf;
	guchar *dst_pixels= dst_pixbuf->get_pixels();
	*dst_img[0].text = "Color image to Gray image(lightness. (max(R,G,B)+min(R,G,B))/2)";

	for (int i = 0; i < opencv_src_img.rows; i++) {
		for (int j = 0; j < opencv_src_img.cols; j++) {
			if (opencv_dst_img.type() == CV_8UC4) {
				cv::Vec4b color = opencv_src_img.at<cv::Vec4b>(cv::Point(j,i));
				color.val[0]= std::min(std::max((std::max(std::max(color.val[0], color.val[1]), color.val[2]) +
								std::min(std::min(color.val[0], color.val[1]), color.val[2])) / 2, 0), 255);
				color.val[1] = color.val[2] = color.val[0];

				opencv_dst_img.at<cv::Vec4b>(cv::Point(j,i)) = color;
			}
			else if (opencv_dst_img.type() == CV_8UC3) {
				cv::Vec3b color = opencv_src_img.at<cv::Vec3b>(cv::Point(j,i));
				color.val[0]= std::min(std::max((std::max(std::max(color.val[0], color.val[1]), color.val[2]) + 
								std::min(std::min(color.val[0], color.val[1]), color.val[2])) / 2, 0), 255);
				color.val[1] = color.val[2] = color.val[0];

				opencv_dst_img.at<cv::Vec3b>(cv::Point(j,i)) = color;
			}
		}
	}

	memcpy(dst_pixels, opencv_dst_img.data, opencv_dst_img.rows * opencv_dst_img.cols * opencv_dst_img.channels());

	//rgb average gray. (R + G + B) / 3
	dst_pixbuf = dst_img[1].pixbuf;
	dst_pixels= dst_pixbuf->get_pixels();
	*dst_img[1].text = "Color image to Gray image(rgb average gray. (R + G + B) / 3)";

	for (int i = 0; i < opencv_src_img.rows; i++) {
		for (int j = 0; j < opencv_src_img.cols; j++) {
			if (opencv_dst_img.type() == CV_8UC4) {
				cv::Vec4b color = opencv_src_img.at<cv::Vec4b>(cv::Point(j,i));
				color.val[0]= (color.val[0]+color.val[1]+color.val[2])/3;
				color.val[1] = color.val[2] = color.val[0];

				opencv_dst_img.at<cv::Vec4b>(cv::Point(j,i)) = color;
			}
			else if (opencv_dst_img.type() == CV_8UC3) {
				cv::Vec3b color = opencv_src_img.at<cv::Vec3b>(cv::Point(j,i));
				color.val[0]= (color.val[0]+color.val[1]+color.val[2])/3;
				color.val[1] = color.val[2] = color.val[0];

				opencv_dst_img.at<cv::Vec3b>(cv::Point(j,i)) = color;
			}
		}
	}
	memcpy(dst_pixels, opencv_dst_img.data, opencv_dst_img.rows * opencv_dst_img.cols * opencv_dst_img.channels());

	//use cvtColor function to convert color to gray.
	dst_pixbuf = dst_img[2].pixbuf;
	dst_pixels= dst_pixbuf->get_pixels();
	*dst_img[2].text = "Color image to Gray image(opencv cvtColor function";

	cv::Mat opencv_dst_gray;
	cv::cvtColor(opencv_src_img, opencv_dst_gray, cv::COLOR_RGB2GRAY);

	guchar *src_pixels= src_pixbuf->get_pixels();
	int size = opencv_dst_gray.rows * opencv_dst_gray.cols;
	for (int i = 0; i < size; i++) {
		if (opencv_dst_img.type() == CV_8UC4) {
			dst_pixels[4 * i] = opencv_dst_gray.data[i];
			dst_pixels[4 * i + 1] = opencv_dst_gray.data[i];
			dst_pixels[4 * i + 2] = opencv_dst_gray.data[i];
			dst_pixels[4 * i + 3] = src_pixels[4 * i + 3];
		}
		else if (opencv_dst_img.type() == CV_8UC3) {
			dst_pixels[3 * i] = opencv_dst_gray.data[i];
			dst_pixels[3 * i + 1] = opencv_dst_gray.data[i];
			dst_pixels[3 * i + 2] = opencv_dst_gray.data[i];
		}
	}

	return MAGNOLIA_SUCCESS;
}

int ImageProcessingOpenCv::image_processing_inversion(magnolia_param_type *cmd, std::vector<pixbuf_label> &src_img, std::vector<pixbuf_label> &dst_img)
{
	Gdk::Pixbuf *src_pixbuf = src_img[0].pixbuf;
	Gdk::Pixbuf *dst_pixbuf = dst_img[0].pixbuf;
	*dst_img[0].text = "Inversion";
	struct gdk_pixbuf_image_info pixbuf_info;

	if (check_and_print_image_info(__FUNCTION__, src_pixbuf, &pixbuf_info) != MAGNOLIA_SUCCESS) {
		return MAGNOLIA_FAILURE;
	}

	cv::Mat opencv_src_img = convert_gdk_pixbuf_to_cv_mat(*src_pixbuf);
	cv::Mat opencv_dst_img;

	bitwise_not(opencv_src_img, opencv_dst_img);
	int len = opencv_src_img.rows * opencv_src_img.cols * opencv_src_img.channels();

	//No invert for alpha channel
	if (pixbuf_info.has_alpha) {
		for (int i = 3; i < len; i+=4) {
			opencv_dst_img.data[i] = ~opencv_dst_img.data[i];
		}
	}

	guchar *dst_pixels= dst_pixbuf->get_pixels();
	memcpy(dst_pixels, opencv_dst_img.data, opencv_dst_img.rows * opencv_dst_img.cols * opencv_dst_img.channels());

	return MAGNOLIA_SUCCESS;
}

int ImageProcessingOpenCv::image_processing_image_binarization(magnolia_param_type *cmd, std::vector<pixbuf_label> &src_img, std::vector<pixbuf_label> &dst_img)
{
	Gdk::Pixbuf *src_pixbuf = src_img[0].pixbuf;
	Gdk::Pixbuf *dst_pixbuf = dst_img[0].pixbuf;
	*dst_img[0].text = "Image binarization : threshold == " + to_string(cmd->param.img_binarization.value);
	struct gdk_pixbuf_image_info pixbuf_info;

	if (check_and_print_image_info(__FUNCTION__, src_pixbuf, &pixbuf_info) != MAGNOLIA_SUCCESS) {
		return MAGNOLIA_FAILURE;
	}

	cv::Mat opencv_src_img = convert_gdk_pixbuf_to_cv_mat(*src_pixbuf);
	cv::Mat opencv_src_gray;
	cv::cvtColor(opencv_src_img, opencv_src_gray, cv::COLOR_RGB2GRAY);
	cv::Mat opencv_dst_img;

	threshold(opencv_src_gray, opencv_dst_img, cmd->param.img_binarization.value, 255, cv::THRESH_BINARY);

	guchar *dst_pixels = dst_pixbuf->get_pixels();

	int size = opencv_dst_img.rows * opencv_dst_img.cols;
	for (int i = 0; i < size; i++) {
		dst_pixels[3 * i] = opencv_dst_img.data[i];
		dst_pixels[3 * i + 1] = opencv_dst_img.data[i];
		dst_pixels[3 * i + 2] = opencv_dst_img.data[i];
	}

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
