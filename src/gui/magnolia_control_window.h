/*
 * =====================================================================================
 *
 *       Filename:  magnolia_control_window.h
 *
 *    Description:  
 *      This file is a magnolia control window header file
 *
 * =====================================================================================
 */

#ifndef _MAGNOLIA_CONTROL_WINDOW_H
#define _MAGNOLIA_CONTROL_WINDOW_H 

#include <thread>
#include <gtkmm.h>
#include "magnolia_common_data.h"

class MagnoliaControlWindow : public Gtk::Window
{
	public:
		MagnoliaControlWindow();
		MagnoliaControlWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
		virtual ~MagnoliaControlWindow();
		void run_thread(magnolia_cmd_param_type &cmd, int result_image_count);
		void on_test();
		void on_button_color_image_to_gray_image_clicked();
		void on_button_inverse_clicked();
		Glib::RefPtr<Gdk::Pixbuf> get_current_image_buf();
		void set_parent_window(Gtk::Window *parent_window);   //add to know parent window in magnolia_contro_window_
		Gtk::Window* get_parent_window(); 

	protected:
	private:
		Glib::RefPtr<Gtk::Builder> magnolia_control_ref_glade_;
		Gtk::Button *p_button_test_ ;
		Gtk::Button *p_button_inverse_;
		Gtk::Button *p_button_color_image_to_gray_image_;
		Gtk::Window *parent_window_;
		mutable std::mutex m_Mutex; 
		std::thread *worker_thread_;
		std::mutex   control_mutex;
};

#endif
