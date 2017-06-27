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
#ifndef _MAGNOLIA_IMAGE_WINDOW_H
#define _MAGNOLIA_IMAGE_WINDOW_H

#include <gtkmm.h>
#include <gdkmm/rgba.h>
#include <gtkmm/grid.h>

struct ImageListStruct
{
	int image_id;
	Gtk::Frame *frame;
	Gtk::EventBox *eventbox;
	Gtk::Image *image;
};

class MagnoliaImageWindow :public Gtk::Window
{
	public:
		MagnoliaImageWindow();
		MagnoliaImageWindow(std::string filename);
		virtual ~MagnoliaImageWindow();
		virtual void on_show();
	private:
		int image_cnt = 0;
		Gtk::Fixed *fixed_;
		std::map<int, ImageListStruct*> image_list_;
};

#endif //_MAGNOLIA_IMAGE_WINDOW_H
