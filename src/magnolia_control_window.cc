/*
 * =====================================================================================
 *
 *       Filename:  magnolia_control_window.cc
 *
 *    Description:  
 *      This file is a magnolia control window source file
 *
 * =====================================================================================
 */


#include "magnolia_control_window.h"
#include "magnolia_image_window.h"
#include <iostream>

using namespace std;

MagnoliaControlWindow::MagnoliaControlWindow()
{ 
}

MagnoliaControlWindow::~MagnoliaControlWindow()
{

}

MagnoliaControlWindow::MagnoliaControlWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
	: Gtk::Window(cobject),
	magnolia_control_ref_glade_(refGlade)
{
	magnolia_control_ref_glade_->get_widget("id_button_test", p_button_test_);
#if 0
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

#endif


	show_all_children();

	p_button_test_->signal_clicked().connect(sigc::mem_fun(*this,
				&MagnoliaControlWindow::on_test));

}


void MagnoliaControlWindow::on_test()
{
	std::cout<<"p_button_test_"<<endl;
}
