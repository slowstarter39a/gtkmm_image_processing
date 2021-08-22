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
#include "magnolia_image_window.h"

class MagnoliaControlWindow : public Gtk::Window
{
	public:
		MagnoliaControlWindow();
		MagnoliaControlWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
		virtual ~MagnoliaControlWindow();
		virtual bool on_focus_out_event(GdkEventFocus* focus_event);
		void run_thread(magnolia_param_type &cmd);
		void on_button_color_image_to_gray_image_clicked();
		void on_button_inverse_clicked();
		void on_checkbutton_image_binarization_clicked();
		void on_scale_image_binarization_value_changed();
		void on_button_histogram_clicked();
		void on_button_close_clicked();
		Glib::RefPtr<Gdk::Pixbuf> get_current_image_buf();
		void set_parent_window(Gtk::Window *parent_window);   //add to know parent window in magnolia_contro_window_
		Gtk::Window* get_parent_window(); 

	protected:
	private:
		Glib::RefPtr<Gtk::Builder> magnolia_control_ref_glade_;
		Gtk::Button *p_button_color_image_to_gray_image_;
		Gtk::Button *p_button_inverse_;
		Gtk::CheckButton *p_checkbutton_image_binarization_;
		Gtk::Scale *p_scale_image_binarization_;
		Gtk::Button *p_button_histogram_;
		Gtk::Button *p_button_close_;
		Gtk::Window *parent_window_;
		mutable std::mutex m_Mutex; 
		std::thread *worker_thread_;
		std::mutex   control_mutex;
		ImageListStruct *dst_image_binarization_;
};

#endif
