/*
 * =====================================================================================
 *
 *       Filename:  main.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/27/2017 09:57:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <gtkmm.h>
#include "magnolia_main_window.h"
#include <iostream>

MagnoliaMainWindow *p_window = nullptr;

int main(int argc, char *argv[])
{
	auto app = Gtk::Application::create(argc, argv, "image processing");

	auto refBuilder = Gtk::Builder::create();
	try
	{
		//refBuilder->add_from_file("../glade/main_ui.glade");
		refBuilder->add_from_file("glade/main_ui.glade");
	}
	catch(const Glib::FileError& ex)
	{
		std::cerr << "FileError: " << ex.what() << std::endl;
		return 1;
	}
	catch(const Glib::MarkupError& ex)
	{
		std::cerr << "MarkupError: " << ex.what() << std::endl;
		return 1;
	}
	catch(const Gtk::BuilderError& ex)
	{
		std::cerr << "BuilderError: " <<ex.what() << std::endl;
		return 1;
	}

	refBuilder->get_widget_derived("main_ui", p_window);
	if(p_window)
	{
#if 0
		Gtk::MenuBar *p_main_menu_bar = nullptr;
		refBuilder->get_widget("main_menu_bar", p_main_menu_bar);

		Gtk::MenuItem *p_main_menu_files = nullptr;
		refBuilder->get_widget("main_menu_files", p_main_menu_files);

		Gtk::MenuItem *p_main_menu_help = nullptr;
		refBuilder->get_widget("main_menu_help", p_main_menu_help);

		Gtk::ImageMenuItem *p_sub_menu_about = nullptr;
		refBuilder->get_widget("sub_menu_about", p_sub_menu_about);

		p_sub_menu_about->signal_activate().connect(sigc::ptr_fun(on_sub_menu_about_activate));
#endif
		app->run(*p_window);
	}

	delete p_window;

	return 0;
}

