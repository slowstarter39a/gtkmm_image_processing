/*
 * =====================================================================================
 *
 *       Filename:  magnolia_main_window.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/01/2017 10:17:11 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef MAGNOLIA_MAIN_WINDOW_H
#define MAGNOLIA_MAIN_WINDOW_H

#include <gtkmm.h>
#include "magnolia_image_window.h"

class MagnoliaMainWindow : public Gtk::Window
{
	public:
		MagnoliaMainWindow();
		MagnoliaMainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
		virtual ~MagnoliaMainWindow();

	protected:
		void on_sub_menu_new_activate();
		void on_sub_menu_open_activate();
		void on_sub_menu_about_activate();
		void on_about_dialog_response(int response_id);
		Glib::RefPtr<Gtk::Builder> magnolia_main_ref_glade_;
		Gtk::AboutDialog magnolia_about_dialog_;
		Gtk::MenuBar *p_main_menu_bar_;
		Gtk::ImageMenuItem *p_sub_menu_new_;
		Gtk::ImageMenuItem *p_sub_menu_open_;
		Gtk::ImageMenuItem *p_sub_menu_about_;
		MagnoliaImageWindow magnolia_image_window_;
		Gtk::Image image2;
};


#endif //MAGNOLIA_MAIN_WINDOW_H
