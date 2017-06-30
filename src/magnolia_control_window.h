/*
 * =====================================================================================
 *
 *       Filename:  magnolia_control_window.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/27/2017 11:00:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _MAGNOLIA_CONTROL_WINDOW_H
#define _MAGNOLIA_CONTROL_WINDOW_H 

#include <gtkmm.h>

class MagnoliaControlWindow : public Gtk::Window
{
	public:
		MagnoliaControlWindow();
		MagnoliaControlWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
		virtual ~MagnoliaControlWindow();

	protected:
	private:
		Glib::RefPtr<Gtk::Builder> magnolia_control_ref_glade_;
		Gtk::Button* p_button_test_ ;
};

#endif
