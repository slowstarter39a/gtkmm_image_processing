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
#include "magnolia_set_image_path_window.h"
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
		MagnoliaXmlStruct *get_magnolia_xml_struct();

	protected:
	private:
		int window_cnt_ = 0;
		Glib::RefPtr<Gtk::Builder> magnolia_main_ref_glade_;
		Gtk::AboutDialog magnolia_about_dialog_;
		Gtk::MenuBar *p_main_menu_bar_;
		Gtk::ImageMenuItem *p_sub_menu_new_;
		Gtk::ImageMenuItem *p_sub_menu_open_;
		Gtk::ImageMenuItem *p_sub_menu_save_;
		Gtk::ImageMenuItem *p_sub_menu_save_as_;
		Gtk::MenuItem *p_sub_menu_image_control_window_;
		Gtk::RadioMenuItem *p_sub_radio_menu_debug_log_level_error_;
		Gtk::RadioMenuItem *p_sub_radio_menu_debug_log_level_warn_;
		Gtk::RadioMenuItem *p_sub_radio_menu_debug_log_level_info_;
		Gtk::RadioMenuItem *p_sub_radio_menu_debug_log_level_debug_;
		Gtk::RadioMenuItem *p_sub_radio_menu_debug_log_level_trace_;
		Gtk::RadioMenuItem *p_sub_radio_menu_lib_type_gtkmm_;
		Gtk::RadioMenuItem *p_sub_radio_menu_lib_type_opencv_;
		Gtk::MenuItem *p_sub_menu_set_default_image_path_;
		Gtk::ImageMenuItem *p_sub_menu_about_;
		MagnoliaImageWindow *current_image_window_ = NULL;
		MagnoliaControlWindow *magnolia_control_window_ = NULL;
		MagnoliaSetImagePathWindow *magnolia_set_image_path_window_ = NULL;
		MagnoliaXmlStruct *magnolia_xml_struct_ = NULL;

		std::map<int, ImageWindowStruct*> image_windows_;

		void on_sub_menu_new_activate();
		void on_sub_menu_open_activate();
		void on_sub_menu_save_activate();
		void on_sub_menu_save_as_activate();
		void on_sub_menu_about_activate();
		void on_sub_menu_image_control_window_activate();
		void on_sub_radio_menu_debug_log_level_set();
		void on_sub_radio_menu_lib_type_set();
		void on_sub_menu_set_default_image_path_activate();
		void on_about_dialog_response(int response_id);
		void on_image_window_close(ImageWindowStruct *image_window);
		void on_image_control_window_close();
		void on_set_default_image_path_window_close();
		void update_ui_memu_items_with_xml_struct();
};


#endif //_MAGNOLIA_MAIN_WINDOW_H
