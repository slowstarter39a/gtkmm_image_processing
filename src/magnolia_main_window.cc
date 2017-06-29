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
#include "magnolia_image_window.h"
#include <iostream>

using namespace std;

MagnoliaMainWindow::MagnoliaMainWindow()
{ 
}

MagnoliaMainWindow::~MagnoliaMainWindow()
{
	std::map<int, ImageWindowStruct*>::iterator iter;
	ImageWindowStruct *img_window = NULL;
	
	for(iter = image_windows_.begin(); iter != image_windows_.end(); iter++){
		img_window = iter->second;

		std::stringstream ss;
		ss << "Deleting Window '" << iter->first <<"'";
		std::cout << ss.str() << std::endl;

		delete img_window->magnolia_image_window;
		delete img_window;
	}

}

MagnoliaMainWindow::MagnoliaMainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
	: Gtk::Window(cobject),
	magnolia_main_ref_glade_(refGlade)
{
	magnolia_main_ref_glade_->get_widget("main_menu_bar", p_main_menu_bar_);

	magnolia_about_dialog_.set_transient_for(*this);

	magnolia_main_ref_glade_->get_widget("sub_menu_new", p_sub_menu_new_);
	magnolia_main_ref_glade_->get_widget("sub_menu_open", p_sub_menu_open_);
	magnolia_main_ref_glade_->get_widget("sub_menu_about", p_sub_menu_about_);

	p_sub_menu_new_->signal_activate().connect(sigc::mem_fun(*this,
				&MagnoliaMainWindow::on_sub_menu_new_activate));

	p_sub_menu_open_->signal_activate().connect(sigc::mem_fun(*this,
				&MagnoliaMainWindow::on_sub_menu_open_activate));

	p_sub_menu_about_->signal_activate().connect(sigc::mem_fun(*this,
				&MagnoliaMainWindow::on_sub_menu_about_activate));

	magnolia_about_dialog_.set_transient_for(*this);
	magnolia_about_dialog_.set_program_name("Magnolia");
	magnolia_about_dialog_.set_version("0.0.0");
	magnolia_about_dialog_.set_copyright("slowstarter80");
	magnolia_about_dialog_.set_comments("This is Magnolia application");
	magnolia_about_dialog_.set_license("LGPL");
	magnolia_about_dialog_.signal_response().connect(sigc::mem_fun(*this, 
				&MagnoliaMainWindow::on_about_dialog_response));

	show_all_children();
}

void MagnoliaMainWindow::on_sub_menu_new_activate(void)
{
	//MagnoliaImageWindow magnolia_image_window;
	//magnolia_image_window_.set_title("Magnolia Image window");
	//magnolia_image_window_.show();
	//magnolia_image_window_.set_deletable(false);
	//magnolia_image_window_.move(400,200);
	std::cout<<"on_sub_menu_new_activate()"<<std::endl;
}

void MagnoliaMainWindow::on_sub_menu_open_activate(void)
{
	Gtk::FileChooserDialog dialog("Please choose a file", 
			Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);

	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Open", Gtk::RESPONSE_OK);

	auto filter_image= Gtk::FileFilter::create();
	filter_image->set_name("Image files");
	filter_image->add_mime_type("image/png");
	filter_image->add_mime_type("image/bmp");
	filter_image->add_mime_type("image/jpg");
	filter_image->add_mime_type("image/jpeg");
	filter_image->add_pattern("*.png");
	filter_image->add_pattern("*.bmp");
	filter_image->add_pattern("*.jpg");
	filter_image->add_pattern("*.jpeg");
	dialog.add_filter(filter_image);

	auto filter_any = Gtk::FileFilter::create();
	filter_any->set_name("Any files");
	filter_any->add_pattern("*");
	dialog.add_filter(filter_any);

	int result = dialog.run();

	switch(result)
	{
		case(Gtk::RESPONSE_OK):
		{
			std::cout<<"Open clicked."<<std::endl;

			std::string filename = dialog.get_filename();
			std::cout<<"File selected: " <<filename <<std::endl;
			ImageWindowStruct *img_window = new ImageWindowStruct;
			img_window->magnolia_image_window = new MagnoliaImageWindow(filename);
			img_window->window_id = window_cnt;

			img_window->magnolia_image_window->set_transient_for(*this);

			std::stringstream ss;
			ss << "New image window '" << window_cnt << "'";
			img_window->magnolia_image_window->set_title(filename);
			img_window->magnolia_image_window->signal_hide().connect( 
				sigc::bind<ImageWindowStruct*>(sigc::mem_fun(*this, 
				&MagnoliaMainWindow::on_image_window_close), img_window));

			image_windows_[window_cnt++] = img_window;
			
			img_window->magnolia_image_window->signal_realize();
			img_window->magnolia_image_window->show();

			//on_sub_menu_new_activate();
			
			break;
		}

		case(Gtk::RESPONSE_CANCEL):
		{
			std::cout<<"Cancel clicked." <<std::endl;
			break;
		}

		default:
		{
			std::cout<<"Unexpected button clicked."<<std::endl;
			break;
		}
	}
	
}

void MagnoliaMainWindow::on_sub_menu_about_activate(void)
{
	magnolia_about_dialog_.show();
	magnolia_about_dialog_.present();
}

void MagnoliaMainWindow::on_about_dialog_response(int response_id)
{
	switch(response_id)
	{
		case Gtk::RESPONSE_CLOSE:
		case Gtk::RESPONSE_CANCEL:
		case Gtk::RESPONSE_DELETE_EVENT:
			magnolia_about_dialog_.hide();
			break;

		default:
			break; 
	}
}

void MagnoliaMainWindow::on_image_window_close(ImageWindowStruct *img_window)
{
	std::stringstream ss;

	ss << "Deleting Window '" << img_window->window_id << "'";
	std::cout << ss.str() << std::endl;
	image_windows_.erase(img_window->window_id);
	delete img_window->magnolia_image_window;
	delete img_window; 
}

void MagnoliaMainWindow::SetCurrentImageWindow(MagnoliaImageWindow* current_window)
{
	current_window_  = current_window; 
	std::stringstream ss;

	ss << "Set current Window '" << current_window_<< "'";
	std::cout << ss.str() << std::endl;
}
