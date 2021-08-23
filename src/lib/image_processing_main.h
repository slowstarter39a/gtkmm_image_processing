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

#ifndef _IMAGE_PROCESSING_MAIN_H
#define _IMAGE_PROCESSING_MAIN_H 

#include <gtkmm.h>
#include <gdkmm/rgba.h>
#include <gtkmm/grid.h>
#include "magnolia_common_data.h"
#include <vector>

class ImageProcessingMain
{
	public:
		virtual ~ImageProcessingMain();
		virtual int image_processing_color_image_to_gray_image(magnolia_param_type *cmd, std::vector<pixbuf_label> &src_img, std::vector<pixbuf_label> &dst_img) = 0;
		virtual int image_processing_inversion(magnolia_param_type *cmd, std::vector<pixbuf_label> &src_img, std::vector<pixbuf_label> &dst_img) = 0;
		virtual int image_processing_image_binarization(magnolia_param_type *cmd, std::vector<pixbuf_label> &src_img, std::vector<pixbuf_label> &dst_img) = 0;
		virtual int image_processing_not_implemented(magnolia_param_type *cmd) = 0;
		magnolia_error_type check_and_print_image_info(const char *function, Gdk::Pixbuf *img_pixbuf, struct gdk_pixbuf_image_info *pixbuf_info);

	protected:
	private:


};

//typedef int image_processing_handler_t(int lib_type, magnolia_param_type *cmd, Gdk::Pixbuf &src_img, Gdk::Pixbuf &dst_img);
typedef int image_processing_handler_t(int lib_type, magnolia_param_type *cmd, std::vector<pixbuf_label> &src_img, std::vector<pixbuf_label> &dst_img);
typedef int image_processing_log_level_handler_t(int log_level);

#define CALL_MEMBER_FUNCTION(obj, ptrToMember) (obj->*ptrToMember)

#endif //_IMAGE_PROCESSING_MAIN_H 
