/*
 * =====================================================================================
 *
 *       Filename:  magnolia_image_window.h
 *
 *    Description:  
 *      This file is a magnolia image window header file
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
		bool OnEventBoxButtonPress(GdkEventButton *, int image_id);
		virtual void OnShow();
		virtual bool OnFocusInEvent(GdkEventFocus* focus_event);
		Glib::RefPtr<Gdk::Pixbuf> GetCurrentImagePixbuf();

	private:
		int image_cnt = 0;
		Gtk::Fixed *fixed_;
		std::map<int, ImageListStruct*> image_list_;
		ImageListStruct *current_img_list_struct_;
};

#endif //_MAGNOLIA_IMAGE_WINDOW_H
