/*
 * =====================================================================================
 *
 *       Filename:  magnolia_main_window.cc
 *
 *    Description:  
 *      This file is a magnolia main window source file
 *
 * =====================================================================================
 */

#include "magnolia_main_window.h"
#include "magnolia_image_window.h"
#include "magnolia_logger.h"
#include <iostream>

using namespace std;
static const char *tag = __FILE__;

MagnoliaMainWindow::MagnoliaMainWindow()
{ 
}

MagnoliaMainWindow::~MagnoliaMainWindow()
{
	std::map<int, ImageWindowStruct*>::iterator iter;
	ImageWindowStruct *img_window = NULL;

	for (iter = image_windows_.begin(); iter != image_windows_.end(); iter++) {
		img_window = iter->second;

		MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "Deleting Window '%d'\n", iter->first);

		delete img_window->magnolia_image_window_;
		delete img_window;
	}

	if (magnolia_control_window_) {
		delete magnolia_control_window_;
	}

	if (magnolia_xml_struct_) {
		delete magnolia_xml_struct_;
	}
}

MagnoliaMainWindow::MagnoliaMainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
	: Gtk::Window(cobject),
	magnolia_main_ref_glade_(refGlade)
{
	magnolia_main_ref_glade_->get_widget("main_menu_bar", p_main_menu_bar_);

	magnolia_main_ref_glade_->get_widget("sub_menu_new", p_sub_menu_new_);
	magnolia_main_ref_glade_->get_widget("sub_menu_open", p_sub_menu_open_);
	magnolia_main_ref_glade_->get_widget("sub_menu_save", p_sub_menu_save_);
	magnolia_main_ref_glade_->get_widget("sub_menu_save_as", p_sub_menu_save_as_);
	magnolia_main_ref_glade_->get_widget("sub_menu_image_control_window", p_sub_menu_image_control_window_);
	magnolia_main_ref_glade_->get_widget("sub_radio_menu_debug_log_level_error", p_sub_radio_menu_debug_log_level_error_);
	magnolia_main_ref_glade_->get_widget("sub_radio_menu_debug_log_level_warn", p_sub_radio_menu_debug_log_level_warn_);
	magnolia_main_ref_glade_->get_widget("sub_radio_menu_debug_log_level_info", p_sub_radio_menu_debug_log_level_info_);
	magnolia_main_ref_glade_->get_widget("sub_radio_menu_debug_log_level_debug", p_sub_radio_menu_debug_log_level_debug_);
	magnolia_main_ref_glade_->get_widget("sub_radio_menu_debug_log_level_trace", p_sub_radio_menu_debug_log_level_trace_);
	magnolia_main_ref_glade_->get_widget("sub_radio_menu_lib_type_gtkmm", p_sub_radio_menu_lib_type_gtkmm_);
	magnolia_main_ref_glade_->get_widget("sub_radio_menu_lib_type_opencv", p_sub_radio_menu_lib_type_opencv_);
	magnolia_main_ref_glade_->get_widget("sub_menu_set_default_image_path", p_sub_menu_set_default_image_path_);
	magnolia_main_ref_glade_->get_widget("sub_menu_about", p_sub_menu_about_);

	p_sub_menu_new_->signal_activate().connect(sigc::mem_fun(*this,
				&MagnoliaMainWindow::on_sub_menu_new_activate));

	p_sub_menu_open_->signal_activate().connect(sigc::mem_fun(*this,
				&MagnoliaMainWindow::on_sub_menu_open_activate));

	p_sub_menu_save_->signal_activate().connect(sigc::mem_fun(*this,
				&MagnoliaMainWindow::on_sub_menu_save_activate));

	p_sub_menu_save_as_->signal_activate().connect(sigc::mem_fun(*this,
				&MagnoliaMainWindow::on_sub_menu_save_as_activate));

	p_sub_menu_image_control_window_->signal_activate().connect(sigc::mem_fun(*this,
				&MagnoliaMainWindow::on_sub_menu_image_control_window_activate));

	p_sub_radio_menu_debug_log_level_error_->signal_activate().connect(sigc::mem_fun(*this,
				&MagnoliaMainWindow::on_sub_radio_menu_debug_log_level_set));

	p_sub_radio_menu_debug_log_level_warn_->signal_activate().connect(sigc::mem_fun(*this,
				&MagnoliaMainWindow::on_sub_radio_menu_debug_log_level_set));

	p_sub_radio_menu_debug_log_level_info_->signal_activate().connect(sigc::mem_fun(*this,
				&MagnoliaMainWindow::on_sub_radio_menu_debug_log_level_set));

	p_sub_radio_menu_debug_log_level_debug_->signal_activate().connect(sigc::mem_fun(*this,
				&MagnoliaMainWindow::on_sub_radio_menu_debug_log_level_set));

	p_sub_radio_menu_debug_log_level_trace_->signal_activate().connect(sigc::mem_fun(*this,
				&MagnoliaMainWindow::on_sub_radio_menu_debug_log_level_set));

	p_sub_radio_menu_lib_type_gtkmm_->signal_activate().connect(sigc::mem_fun(*this,
				&MagnoliaMainWindow::on_sub_radio_menu_lib_type_set));

	p_sub_radio_menu_lib_type_opencv_->signal_activate().connect(sigc::mem_fun(*this,
				&MagnoliaMainWindow::on_sub_radio_menu_lib_type_set));

	p_sub_menu_set_default_image_path_->signal_activate().connect(sigc::mem_fun(*this,
				&MagnoliaMainWindow::on_sub_menu_set_default_image_path_activate));

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

	magnolia_xml_struct_ = new MagnoliaXmlStruct;
	magnolia_xml_struct_->read_xml();

	MGNL_SET_LOG_LEVEL(magnolia_xml_struct_->get_log_level());
	update_ui_memu_items_with_xml_struct();
}

void MagnoliaMainWindow::on_sub_menu_new_activate(void)
{
	//MagnoliaImageWindow magnolia_image_window;
	//magnolia_image_window_.set_title("Magnolia Image window");
	//magnolia_image_window_.show();
	//magnolia_image_window_.set_deletable(false);
	//magnolia_image_window_.move(400,200);
	MGNL_PRINTF(tag, LOG_LEVEL_TRACE, "%s\n", __FUNCTION__);
}

void MagnoliaMainWindow::on_sub_menu_open_activate(void)
{
	Gtk::FileChooserDialog dialog("Please choose a file", 
			Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);

	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Open", Gtk::RESPONSE_OK);
	dialog.set_current_folder(magnolia_xml_struct_->get_default_image_path());

	int size = Gdk::Pixbuf::get_formats().size();
	char buf[30] = {0,};
	auto filter_image = Gtk::FileFilter::create();
	filter_image->set_name("Image files");

	for (int i = 0; i < size; i++) {
		if (Gdk::Pixbuf::get_formats()[i].is_writable()) {
			memset(buf, 0x0, sizeof(buf));
			memcpy(buf, "image/", strlen("image/") + 1);
			if (!memcmp(Gdk::Pixbuf::get_formats()[i].get_name().c_str(), "ico", strlen("ico") + 1)) {
				strncat(buf, "x-icon", strlen("x-icon") + 1);
			}
			else {
				int len = strlen(Gdk::Pixbuf::get_formats()[i].get_name().c_str()) + 1;
				strncat(buf, Gdk::Pixbuf::get_formats()[i].get_name().c_str(), len);
			}
			filter_image->add_mime_type(buf);
			filter_image->add_pattern("*." + Gdk::Pixbuf::get_formats()[i].get_name());
		}
	}

	dialog.add_filter(filter_image);

	auto filter_any = Gtk::FileFilter::create();
	filter_any->set_name("Any files");
	filter_any->add_pattern("*");
	dialog.add_filter(filter_any);

	int result = dialog.run();

	switch (result) {
		case (Gtk::RESPONSE_OK):
			{
				MGNL_PRINTF(tag, LOG_LEVEL_TRACE, "Open clicked.\n");

				std::string filename = dialog.get_filename();
				MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "File selected: %s\n", filename.c_str());
				ImageWindowStruct *img_window = new ImageWindowStruct;
				img_window->magnolia_image_window_ = new MagnoliaImageWindow(this, filename);
				img_window->window_id_ = window_cnt_;

				img_window->magnolia_image_window_->set_title(filename);
				img_window->magnolia_image_window_->signal_hide().connect(
						sigc::bind<ImageWindowStruct*>(sigc::mem_fun(*this, 
								&MagnoliaMainWindow::on_image_window_close), img_window));

				image_windows_[window_cnt_++] = img_window;

				img_window->magnolia_image_window_->signal_realize();
				img_window->magnolia_image_window_->show();
				current_image_window_  = img_window->magnolia_image_window_;

				break;
			}

		case (Gtk::RESPONSE_CANCEL):
			{
				MGNL_PRINTF(tag, LOG_LEVEL_TRACE, "Cancel clicked.\n");
				break;
			}

		default:
			{
				MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "Unexpected button clicked.\n");
				break;
			}
	}
}

void MagnoliaMainWindow::on_sub_menu_save_activate(void)
{
	//TODO
	//For now, we temporarily implemented save() and save_as() the same.
	if (current_image_window_ != nullptr) {
		current_image_window_->on_popup_menu_save_as_event();
	}
}

void MagnoliaMainWindow::on_sub_menu_save_as_activate(void)
{
	if (current_image_window_ != nullptr) {
		current_image_window_->on_popup_menu_save_as_event();
	}
}

void MagnoliaMainWindow::set_current_image_window(MagnoliaImageWindow* current_window)
{
	current_image_window_  = current_window; 
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "set current window %p\n", current_image_window_);
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
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "Deleting Window '%d'\n", img_window->window_id_);
	image_windows_.erase(img_window->window_id_);
	delete img_window->magnolia_image_window_;
	delete img_window; 
}

void MagnoliaMainWindow::on_sub_menu_image_control_window_activate(void)
{
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "%s\n", __FUNCTION__);

	auto refBuilder = Gtk::Builder::create();
	try
	{
		refBuilder->add_from_file("glade/control_window_ui.glade");
	}
	catch(const Glib::FileError& ex)
	{
		std::cerr << "FileError: " << ex.what() << std::endl;
		return;
	}
	catch(const Glib::MarkupError& ex)
	{
		std::cerr << "MarkupError: " << ex.what() << std::endl;
		return;
	}
	catch(const Gtk::BuilderError& ex)
	{
		std::cerr << "BuilderError: " <<ex.what() << std::endl;
		return;
	}

	refBuilder->get_widget_derived("id_control_window_ui", magnolia_control_window_);

	magnolia_control_window_->set_parent_window(this);   //add to know parent window in magnolia_contro_window_
	magnolia_control_window_->set_title("Image Control Window");
	magnolia_control_window_->signal_realize();

	magnolia_control_window_->signal_hide().connect(
			sigc::mem_fun(*this, &MagnoliaMainWindow::on_image_control_window_close)) ;

	magnolia_control_window_->show();
	magnolia_control_window_->present();
	//magnolia_control_window_->set_transient_for(*this);
}

void MagnoliaMainWindow::on_sub_menu_set_default_image_path_activate()
{
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "%s\n", __FUNCTION__);

	auto refBuilder = Gtk::Builder::create();
	try
	{
		refBuilder->add_from_file("glade/default_image_path_ui.glade");
	}
	catch(const Glib::FileError& ex)
	{
		std::cerr << "FileError: " << ex.what() << std::endl;
		return;
	}
	catch(const Glib::MarkupError& ex)
	{
		std::cerr << "MarkupError: " << ex.what() << std::endl;
		return;
	}
	catch(const Gtk::BuilderError& ex)
	{
		std::cerr << "BuilderError: " <<ex.what() << std::endl;
		return;
	}

	refBuilder->get_widget_derived("id_default_image_path_ui", magnolia_set_image_path_window_);
	magnolia_set_image_path_window_->set_title("Set default Image Path Window");
	magnolia_set_image_path_window_->signal_realize();
	magnolia_set_image_path_window_->set_image_path(magnolia_xml_struct_->get_default_image_path());

	magnolia_set_image_path_window_->set_parent_window(this);   //add to know parent window

	magnolia_set_image_path_window_->signal_hide().connect(
			sigc::mem_fun(*this, &MagnoliaMainWindow::on_set_default_image_path_window_close)) ;
	//magnolia_set_image_path_window_->set_transient_for(*this);

	int result = magnolia_set_image_path_window_->run();

	switch(result) {
		case Gtk::RESPONSE_OK:
			magnolia_xml_struct_->set_default_image_path(magnolia_set_image_path_window_->get_image_path());
			MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "default_image_path changed to %s\n", magnolia_xml_struct_->get_default_image_path());
			break;

		case Gtk::RESPONSE_CANCEL:
			break;

		default:
			break;
	}
	magnolia_set_image_path_window_->hide();
}

//When selected radiomenuitem changed, this function called twice.
void MagnoliaMainWindow::on_sub_radio_menu_debug_log_level_set()
{
	static unsigned int debug_log_level_set_call_count = 0;
	debug_log_level_set_call_count++;
	int log_level = 0;

	if (debug_log_level_set_call_count % 2) {
		if (p_sub_radio_menu_debug_log_level_error_->get_active()) {
			log_level = 0;
		}
		else if (p_sub_radio_menu_debug_log_level_warn_->get_active()) {
			log_level = 1;
		}
		else if (p_sub_radio_menu_debug_log_level_info_->get_active()) {
			log_level = 2;
		}
		else if (p_sub_radio_menu_debug_log_level_debug_->get_active()) {
			log_level = 3;
		}
		else if (p_sub_radio_menu_debug_log_level_trace_->get_active()) {
			log_level = 4;
		}
		magnolia_xml_struct_->set_log_level(log_level);
		MGNL_SET_LOG_LEVEL(magnolia_xml_struct_->get_log_level());
	}
}

//When selected radiomenuitem changed, this function called twice.
void MagnoliaMainWindow::on_sub_radio_menu_lib_type_set()
{
	static unsigned int lib_type_set_call_count = 0;
	lib_type_set_call_count++;
	int lib_type = 0;

	if (lib_type_set_call_count % 2) {
		if (p_sub_radio_menu_lib_type_gtkmm_->get_active()) {
			lib_type = 0;
		}
		else if (p_sub_radio_menu_lib_type_opencv_->get_active()) {
			lib_type = 1;
		}
		magnolia_xml_struct_->set_lib_type(lib_type);
	}
}

void MagnoliaMainWindow::on_image_control_window_close()
{
	MGNL_PRINTF(tag, LOG_LEVEL_TRACE, "%s\n", __FUNCTION__);
	delete magnolia_control_window_;
	magnolia_control_window_ = NULL;
}

void MagnoliaMainWindow::on_set_default_image_path_window_close()
{
	MGNL_PRINTF(tag, LOG_LEVEL_TRACE, "%s\n", __FUNCTION__);
	delete magnolia_set_image_path_window_;
	magnolia_set_image_path_window_= NULL;
}

MagnoliaImageWindow* MagnoliaMainWindow::get_current_image_window()
{
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "current_image_window = %p\n", current_image_window_);
	return current_image_window_;
}

MagnoliaXmlStruct* MagnoliaMainWindow::get_magnolia_xml_struct()
{
	return magnolia_xml_struct_;
}

void MagnoliaMainWindow::update_ui_memu_items_with_xml_struct()
{
	int log_level = magnolia_xml_struct_->get_log_level();

	switch (log_level) {
		case 0:
			p_sub_radio_menu_debug_log_level_error_->set_active();
			break;

		case 1:
			p_sub_radio_menu_debug_log_level_warn_->set_active();
			break;

		case 2:
			p_sub_radio_menu_debug_log_level_info_->set_active();
			break;

		case 3:
			p_sub_radio_menu_debug_log_level_debug_->set_active();
			break;

		case 4:
			p_sub_radio_menu_debug_log_level_trace_->set_active();
			break;

		default:
			p_sub_radio_menu_debug_log_level_error_->set_active();
			break;
	}

	int lib_type = magnolia_xml_struct_->get_lib_type();

	switch (lib_type) {
		case 0:
			p_sub_radio_menu_lib_type_gtkmm_->set_active();
			break;

		case 1:
			p_sub_radio_menu_lib_type_opencv_->set_active();
			break;

		default:
			p_sub_radio_menu_lib_type_gtkmm_->set_active();
			break;
	}
}
