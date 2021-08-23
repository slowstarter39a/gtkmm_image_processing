/*
   Copyright 2017 slowstarter80

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "image_processing_main.h"
#include "image_processing_magnolia.h"
#include "image_processing_opencv.h"
#include "magnolia_logger.h"

using namespace std;
static const char *tag = __FILE__;

extern "C" int ImageProcessingDispatcher(int lib_type, magnolia_param_type *cmd, std::vector<pixbuf_label> &src_img, std::vector<pixbuf_label> &dst_img);
extern "C" int ImageProcessingSetLogLevel(int log_level);

magnolia_image_processing_handler magnolia_func_handler[MAGNOLIA_CMD_MAX] =
{
	&ImageProcessingMagnolia::image_processing_color_image_to_gray_image,    //MAGNOLIA_CMD_COLOR_IMAGE_TO_GRAY_IMAGE,
	&ImageProcessingMagnolia::image_processing_inversion,    				 //MAGNOLIA_CMD_INVERSION,
	&ImageProcessingMagnolia::image_processing_image_binarization,			 //MAGNOLIA_CMD_IMAGE_BINARIZATION,
	NULL, 																	 //MAGNOLIA_CMD_HISTOGRAM,
};

opencv_image_processing_handler opencv_func_handler[MAGNOLIA_CMD_MAX] =
{
	&ImageProcessingOpenCv::image_processing_color_image_to_gray_image,      //MAGNOLIA_CMD_COLOR_IMAGE_TO_GRAY_IMAGE,
	&ImageProcessingOpenCv::image_processing_inversion, 	 				 //MAGNOLIA_CMD_INVERSION,
	&ImageProcessingOpenCv::image_processing_image_binarization,			 //MAGNOLIA_CMD_IMAGE_BINARIZATION,
	NULL, 																	 //MAGNOLIA_CMD_HISTOGRAM,
};

int ImageProcessingDispatcher(int lib_type, magnolia_param_type *cmd, std::vector<pixbuf_label> &src_img, std::vector<pixbuf_label> &dst_img)
{
	int result = MAGNOLIA_FAILURE;
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "ImageProcessingMain ImageProcessingDispatcher! lib_type = %d\n", lib_type);

	if (lib_type == 0) {
		ImageProcessingMagnolia *magnolia_processing = new ImageProcessingMagnolia;
		if (sizeof(magnolia_func_handler) / sizeof(magnolia_func_handler[0]) > cmd->mag_cmd &&
				magnolia_func_handler[cmd->mag_cmd]) {
			result = CALL_MEMBER_FUNCTION(magnolia_processing, magnolia_func_handler[cmd->mag_cmd])(cmd, src_img, dst_img);
		}
		else {
			magnolia_processing->image_processing_not_implemented(cmd);
		}
		delete magnolia_processing;
		return result;
	}
	else if (lib_type == 1) {
		ImageProcessingOpenCv *opencv_processing = new ImageProcessingOpenCv;
		if (sizeof(opencv_func_handler) / sizeof(opencv_func_handler[0]) > cmd->mag_cmd &&
					opencv_func_handler[cmd->mag_cmd]) {
			result = CALL_MEMBER_FUNCTION(opencv_processing, opencv_func_handler[cmd->mag_cmd])(cmd, src_img, dst_img);
		}
		else {
			opencv_processing->image_processing_not_implemented(cmd);
		}
		delete opencv_processing; 
		return result; 
	}

	return MAGNOLIA_FAILURE;
}

extern "C" int ImageProcessingSetLogLevel(int log_level)
{
	MagnoliaLogger *logger = MagnoliaLogger::get_instance();
	logger->set_log_level((log_message_level)log_level);

	return 0;
}

ImageProcessingMain::~ImageProcessingMain()
{
}

magnolia_error_type ImageProcessingMain::check_and_print_image_info(const char *function, Gdk::Pixbuf *img_pixbuf, struct gdk_pixbuf_image_info *pixbuf_info)
{
	pixbuf_info->row_stride = img_pixbuf->get_rowstride();
	pixbuf_info->n_channels = img_pixbuf->get_n_channels();
	pixbuf_info->width = img_pixbuf->get_width();
	pixbuf_info->height = img_pixbuf->get_height();
	pixbuf_info->has_alpha = img_pixbuf->get_has_alpha();
	pixbuf_info->colorspace = img_pixbuf->get_colorspace();
	pixbuf_info->bits_per_sample = img_pixbuf->get_bits_per_sample();

	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "function   = %s\n", function);
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "row_stride = %d\n", img_pixbuf->get_rowstride());
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "n_channels = %d\n", img_pixbuf->get_n_channels());
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "has_alpha  = %d\n", img_pixbuf->get_has_alpha());
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "width      = %d\n", img_pixbuf->get_width());
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "height     = %d\n", img_pixbuf->get_height());

	if (pixbuf_info->colorspace != Gdk::COLORSPACE_RGB) {
		MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "colorspace = %d\n", pixbuf_info->colorspace);
		return MAGNOLIA_FAILURE;
	}
	if (pixbuf_info->bits_per_sample != 8) {
		MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "bits_per_sample = %d\n", pixbuf_info->bits_per_sample);
		return MAGNOLIA_FAILURE;
	}

	return MAGNOLIA_SUCCESS;
}

