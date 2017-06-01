/*
 * =====================================================================================
 *
 *       Filename:  magnolia_main_window.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/01/2017 10:16:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "magnolia_main_window.h"

MagnoliaMainWindow::MagnoliaMainWindow()
{ 
	;
}

MagnoliaMainWindow::MagnoliaMainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
	: Gtk::Window(cobject)
{
}

MagnoliaMainWindow::~MagnoliaMainWindow()
{

}

