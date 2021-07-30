/*
 * =====================================================================================
 *
 *       Filename:  magnolia_set_image_path_window.h
 *
 *    Description:  
 *      This file is a magnolia set image path window header file
 *
 * =====================================================================================
 */

#ifndef _MAGNOLIA_SET_IMAGE_PATH_WINDOW_H
#define _MAGNOLIA_SET_IMAGE_PATH_WINDOW_H

#include <thread>
#include <gtkmm.h>

class MagnoliaSetImagePathWindow : public Gtk::Dialog
{
	public:
		MagnoliaSetImagePathWindow(const char *path);
		MagnoliaSetImagePathWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
		virtual ~MagnoliaSetImagePathWindow();
		void on_button_file_chooser_clicked();
		void set_image_path(const char *path);
		void set_parent_window(Gtk::Window *parent_window);   //add to know parent window
		char* get_image_path();
		Gtk::Window* get_parent_window(); 

	protected:
	private:
		Glib::RefPtr<Gtk::Builder> magnolia_set_image_path_ref_glade_;
		Gtk::Entry *p_entry_;
		Gtk::Button *p_button_file_chooser_;
		Gtk::Window *parent_window_;
		char image_path_[300];
};

#endif
