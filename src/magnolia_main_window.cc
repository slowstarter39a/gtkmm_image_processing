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

MagnoliaMainWindow::MagnoliaMainWindow()
{ 
}

MagnoliaMainWindow::MagnoliaMainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
	: Gtk::Window(cobject),
	magnolia_main_ref_glade_(refGlade)
{
//	Gtk::MenuBar *p_main_menu_bar = nullptr;
	magnolia_main_ref_glade_->get_widget("main_menu_bar", p_main_menu_bar_);

	magnolia_about_dialog_.set_transient_for(*this);

	//Gtk::ImageMenuItem *p_sub_menu_about = nullptr;
	magnolia_main_ref_glade_->get_widget("sub_menu_new", p_sub_menu_new_);
	magnolia_main_ref_glade_->get_widget("sub_menu_about", p_sub_menu_about_);

	p_sub_menu_new_->signal_activate().connect(sigc::mem_fun(*this,
				&MagnoliaMainWindow::on_sub_menu_new_activate));

	p_sub_menu_about_->signal_activate().connect(sigc::mem_fun(*this,
				&MagnoliaMainWindow::on_sub_menu_about_activate));

	magnolia_about_dialog_.set_transient_for(*this);
	/*  
	magnolia_about_dialog_.set_logo(Gdk::Pixbuf::create_from_resource(
				"/about/gtkmm_logo.gif", -1, 40, true));
	*/
	magnolia_about_dialog_.set_program_name("Magnolia");
	magnolia_about_dialog_.set_version("0.0.0");
	magnolia_about_dialog_.set_copyright("slowstarter80");
	magnolia_about_dialog_.set_comments("This is Magnolia application");
	magnolia_about_dialog_.set_license("LGPL");
//magnolia_about_dialog_.set_authors("slowstarter80");
	magnolia_about_dialog_.signal_response().connect(sigc::mem_fun(*this, 
				&MagnoliaMainWindow::on_about_dialog_response));

	show_all_children();
}

MagnoliaMainWindow::~MagnoliaMainWindow()
{

}

void MagnoliaMainWindow::on_sub_menu_new_activate(void)
{
	//MagnoliaImageWindow magnolia_image_window_;
	magnolia_image_window_.set_title("Magnolia Image window");
	magnolia_image_window_.show();
	//magnolia_image_window_.set_deletable(false);
	//magnolia_image_window_.move(400,200);
	std::cout<<"on_sub_menu_new_activate()"<<std::endl;
}

void MagnoliaMainWindow::on_sub_menu_about_activate(void)
{
//	std::cout<<"About menu selected()!"<<std::endl;
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
