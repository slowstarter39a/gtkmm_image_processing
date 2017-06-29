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
#ifndef _MAGNOLIA_MAIN_WINDOW_H
#define _MAGNOLIA_MAIN_WINDOW_H

#include <gtkmm.h>
#include "magnolia_image_window.h"

struct ImageWindowStruct
{
	int window_id;
	MagnoliaImageWindow *magnolia_image_window;
};

class MagnoliaMainWindow : public Gtk::Window
{
	public:
		MagnoliaMainWindow();
		MagnoliaMainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
		virtual ~MagnoliaMainWindow();
		virtual void SetCurrentImageWindow(MagnoliaImageWindow* current_window);

	protected:
	private:
		int window_cnt = 0;
		Glib::RefPtr<Gtk::Builder> magnolia_main_ref_glade_;
		Gtk::AboutDialog magnolia_about_dialog_;
		Gtk::MenuBar *p_main_menu_bar_;
		Gtk::ImageMenuItem *p_sub_menu_new_;
		Gtk::ImageMenuItem *p_sub_menu_open_;
		Gtk::ImageMenuItem *p_sub_menu_about_;
		MagnoliaImageWindow *current_window_ = NULL;

		std::map<int, ImageWindowStruct*> image_windows_;

		void on_sub_menu_new_activate();
		void on_sub_menu_open_activate();
		void on_sub_menu_about_activate();
		void on_about_dialog_response(int response_id);
		void on_image_window_close(ImageWindowStruct *image_window);
		
};


#endif //_MAGNOLIA_MAIN_WINDOW_H
