/*
 * =====================================================================================
 *
 *       Filename:  magnolia_image_window.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/03/2017 10:56:13 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef MAGNOLIA_IMAGE_WINDOW_H
#define MAGNOLIA_IMAGE_WINDOW_H

#include <gtkmm.h>

struct ImageListStruct
{
	int image_id;
	Gtk::Image *image;
};

class MagnoliaImageWindow :public Gtk::Window
{
	public:
		MagnoliaImageWindow();
		MagnoliaImageWindow(std::string filename);
		virtual ~MagnoliaImageWindow();
	private:
		int image_cnt = 0;
		std::map<int, Gtk::Image*> image_list_;
};

#endif //MAGNOLIA_IMAGE_WINDOW_H
