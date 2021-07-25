/*
   Copyright 2017 slowstarter80

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/*
 * =====================================================================================
 *
 *	 Filename:  main.cc
 *
 *    Description:  
 *    	This file is a main.cc of magnolia
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
	try {
		//refBuilder->add_from_file("../glade/main_ui.glade");
		refBuilder->add_from_file("glade/main_ui.glade");
	}
	catch (const Glib::FileError& ex) {
		std::cerr << "FileError: " << ex.what() << std::endl;
		return 1;
	}
	catch (const Glib::MarkupError& ex) {
		std::cerr << "MarkupError: " << ex.what() << std::endl;
		return 1;
	}
	catch (const Gtk::BuilderError& ex) {
		std::cerr << "BuilderError: " <<ex.what() << std::endl;
		return 1;
	}

	refBuilder->get_widget_derived("main_ui", p_window);
	if (p_window) {
		app->run(*p_window);
	}

	delete p_window;

	return 0;
}

