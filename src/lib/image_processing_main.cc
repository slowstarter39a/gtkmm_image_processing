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

extern "C" int ImageProcessingDispatcher(int lib_type, magnolia_cmd_param_type *cmd, Gdk::Pixbuf &src_img, Gdk::Pixbuf &dst_img);
extern "C" int ImageProcessingSetLogLevel(int log_level);

magnolia_image_processing_handler magnolia_func_handler[MAGNOLIA_CMD_MAX] =
{
			&ImageProcessingMagnolia::image_processing_inversion,
			&ImageProcessingMagnolia::image_processing_inversion
};

opencv_image_processing_handler opencv_func_handler[MAGNOLIA_CMD_MAX] =
{
			&ImageProcessingOpenCv::image_processing_inversion,
			&ImageProcessingOpenCv::image_processing_inversion
};

int ImageProcessingDispatcher(int lib_type, magnolia_cmd_param_type *cmd, Gdk::Pixbuf &src_img, Gdk::Pixbuf &dst_img)
{
	int result = MAGNOLIA_FAILURE;
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "ImageProcessingMain ImageProcessingDispatcher! lib_type = %d\n", lib_type);

	if (lib_type == 0) {
		ImageProcessingMagnolia *magnolia_processing = new ImageProcessingMagnolia;
		if (magnolia_func_handler[cmd->mag_cmd]) {
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
		if (opencv_func_handler[cmd->mag_cmd]) {
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
