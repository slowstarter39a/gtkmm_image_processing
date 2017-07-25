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

class ImageProcessingMain
{
	public:
		virtual int lib_func()=0;
	protected:
	private:


};

typedef ImageProcessingMain* image_processing_handler_t(int lib_type, magnolia_cmd_type *cmd, Gdk::Pixbuf &src_img, Gdk::Pixbuf &dst_img);

#endif //_IMAGE_PROCESSING_MAIN_H 
