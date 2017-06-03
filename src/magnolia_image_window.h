/*
 * =====================================================================================
 *
 *       Filename:  magnolia_image_window.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/03/2017 10:56:13 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef MAGNOLIA_IMAGE_WINDOW_H
#define MAGNOLIA_IMAGE_WINDOW_H

#include <gtkmm.h>

class MagnoliaImageWindow :public Gtk::Window
{
	public:
		MagnoliaImageWindow();
		virtual ~MagnoliaImageWindow();
};

#endif //MAGNOLIA_IMAGE_WINDOW_H
