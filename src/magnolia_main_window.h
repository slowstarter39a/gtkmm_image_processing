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

class MagnoliaMainWindow : public Gtk::Window
{
	public:
		MagnoliaMainWindow();
		MagnoliaMainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
		virtual ~MagnoliaMainWindow();
};


#endif //MAGNOLIA_MAIN_WINDOW_H
