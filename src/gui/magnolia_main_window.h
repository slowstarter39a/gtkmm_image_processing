/*
 * =====================================================================================
 *
 *       Filename:  magnolia_main_window.h
 *
 *    Description:  
 *	This file is a magnolia main window header file
 *
 * =====================================================================================
 */
#ifndef _MAGNOLIA_MAIN_WINDOW_H
#define _MAGNOLIA_MAIN_WINDOW_H

#include <gtkmm.h>
#include "magnolia_image_window.h"
#include "magnolia_control_window.h"
#include "magnolia_xml_struct.h"

struct ImageWindowStruct
{
	int window_id_;
	MagnoliaImageWindow *magnolia_image_window_;
};

class MagnoliaMainWindow : public Gtk::Window
{
	public:
		MagnoliaMainWindow();
		MagnoliaMainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
		virtual ~MagnoliaMainWindow();
		virtual void set_current_image_window(MagnoliaImageWindow* current_window);
		MagnoliaImageWindow* get_current_image_window();
		int get_check_menu_use_opencv_lib();

	protected:
	private:
		int window_cnt_ = 0;
		Glib::RefPtr<Gtk::Builder> magnolia_main_ref_glade_;
		Gtk::AboutDialog magnolia_about_dialog_;
		Gtk::MenuBar *p_main_menu_bar_;
		Gtk::ImageMenuItem *p_sub_menu_new_;
		Gtk::ImageMenuItem *p_sub_menu_open_;
		Gtk::MenuItem *p_sub_menu_image_control_window_;
		Gtk::CheckMenuItem *p_sub_menu_use_opencv_lib_;
		Gtk::ImageMenuItem *p_sub_menu_about_;
		MagnoliaImageWindow *current_image_window_ = NULL;
		MagnoliaControlWindow *magnolia_control_window_ = NULL;
		MagnoliaXmlStruct *magnolia_xml_struct_ = NULL;

		std::map<int, ImageWindowStruct*> image_windows_;

		void on_submenu_new_activate();
		void on_submenu_open_activate();
		void on_submenu_about_activate();
		void on_submenu_image_control_window_activate();
		void on_submenu_use_opencv_lib_activate(void);
		void on_about_dialog_response(int response_id);
		void on_image_window_close(ImageWindowStruct *image_window);
		void on_image_control_window_close();
};


#endif //_MAGNOLIA_MAIN_WINDOW_H
