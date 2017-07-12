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
		MagnoliaImageWindow* GetCurrentImageWindow();

	protected:
	private:
		int window_cnt = 0;
		Glib::RefPtr<Gtk::Builder> magnolia_main_ref_glade_;
		Gtk::AboutDialog magnolia_about_dialog_;
		Gtk::MenuBar *p_main_menu_bar_;
		Gtk::ImageMenuItem *p_sub_menu_new_;
		Gtk::ImageMenuItem *p_sub_menu_open_;
		Gtk::MenuItem *p_sub_menu_image_control_window_;
		Gtk::ImageMenuItem *p_sub_menu_about_;
		MagnoliaImageWindow *current_image_window_ = NULL;
		MagnoliaControlWindow *magnolia_control_window_ = NULL;

		std::map<int, ImageWindowStruct*> image_windows_;

		void OnSubmenuNewActivate();
		void OnSubmenuOpenActivate();
		void OnSubmenuAboutActivate();
		void OnSubmenuImageControlWindowActivate();
		void OnAboutDialogResponse(int response_id);
		void OnImageWindowClose(ImageWindowStruct *image_window);
		void OnImageControlWindowClose();
		
};


#endif //_MAGNOLIA_MAIN_WINDOW_H
