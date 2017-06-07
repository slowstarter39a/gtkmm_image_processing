/*
 * =====================================================================================
 *
 *       Filename:  magnolia_image_window.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/03/2017 10:58:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "magnolia_image_window.h"
#include <iostream>

MagnoliaImageWindow::MagnoliaImageWindow()
{
	set_border_width(10);
	set_default_size(500, 500);

	show_all_children();

}
MagnoliaImageWindow::MagnoliaImageWindow(std::string filename)
{
	std::cout<<filename<<std::endl;
	set_border_width(10);
	set_default_size(500, 500);

	Gtk::Image *image = new Gtk::Image;
	image->set(filename);
	add(*image);
	image->show();

	show_all_children();
}

MagnoliaImageWindow::~MagnoliaImageWindow()
{

}
