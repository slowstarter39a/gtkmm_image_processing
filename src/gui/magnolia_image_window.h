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
	Gtk::VBox *vbox;
	Gtk::Image *image;
	Gtk::Label *label;
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
		Glib::RefPtr<Gdk::Pixbuf> get_current_image_pixbuf();
		Glib::ustring get_current_image_label_text();
		void show_dst_image(Glib::RefPtr<Gdk::Pixbuf> &dst_buf, Glib::ustring label_text, int result_image_count);
		bool on_eventbox_button_motion_notify(GdkEventMotion* event);
		void set_popup_menu();
		void on_popup_menu_save_as_event();
		void on_popup_menu_copy_event();
		void on_popup_menu_cut_event();
		void show_not_implemented_yet_messagedialog();
		void on_file_dialog_response(int response_id, Gtk::FileChooserDialog *dialog);

	private:
		int image_cnt_ = 0;
		Gtk::Fixed *fixed_;
		std::map<int, ImageListStruct*> image_list_;
		ImageListStruct *current_img_list_struct_;
		Gtk::Window *parent_window_;
		MousePoint mouse_button_press_;
		Gtk::Menu popup_menu_;
		Gtk::MenuItem menu_item_save_as_;
		Gtk::MenuItem menu_item_cut_;
		Gtk::MenuItem menu_item_copy_;
};

#endif //_MAGNOLIA_IMAGE_WINDOW_H
