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

struct MousePoint
{
	int x;
	int y;
};

class MagnoliaImageWindow :public Gtk::Window
{
	public:
		MagnoliaImageWindow(Gtk::Window *parent_window);
		MagnoliaImageWindow(Gtk::Window *parent_window, std::string filename);
		virtual ~MagnoliaImageWindow();
		bool on_eventbox_button_press(GdkEventButton *event, int image_id);
		virtual void on_show();
		virtual bool on_focus_in_event(GdkEventFocus* focus_event);
		Glib::RefPtr<Gdk::Pixbuf> get_src_image_pixbuf();
		void show_dst_image(Glib::RefPtr<Gdk::Pixbuf> &dst_buf, int dst_img_widht, int dst_img_height);
		bool on_eventbox_button_motion_notify(GdkEventMotion* event);

	private:
		int image_cnt_ = 0;
		Gtk::Fixed *fixed_;
		std::map<int, ImageListStruct*> image_list_;
		ImageListStruct *current_img_list_struct_;
		Gtk::Window *parent_window_;
		MousePoint mouse_button_press_;
};

#endif //_MAGNOLIA_IMAGE_WINDOW_H
