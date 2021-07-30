/*
 * =====================================================================================
 *
 *       Filename:  magnolia_set_image_path_window.cc
 *
 *    Description:  
 *      This file is a magnolia set image_path window source file
 *
 * =====================================================================================
 */

#include "magnolia_set_image_path_window.h"
#include "magnolia_logger.h"

using namespace std;
static const char *tag = __FILE__;

MagnoliaSetImagePathWindow::~MagnoliaSetImagePathWindow()
{

}

MagnoliaSetImagePathWindow::MagnoliaSetImagePathWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
	: Gtk::Dialog(cobject),
	magnolia_set_image_path_ref_glade_(refGlade)
{
	magnolia_set_image_path_ref_glade_->get_widget("id_set_image_path_button_file_chooser", p_button_file_chooser_);
	magnolia_set_image_path_ref_glade_->get_widget("id_set_image_path_entry", p_entry_);

	p_button_file_chooser_->signal_clicked().connect(sigc::mem_fun(*this, &MagnoliaSetImagePathWindow::on_button_file_chooser_clicked)); 

	show_all_children(); 
} 

void MagnoliaSetImagePathWindow::on_button_file_chooser_clicked()
{
	Gtk::FileChooserDialog dialog("Please choose a folder",Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Open", Gtk::RESPONSE_OK);
	dialog.set_transient_for(*this);

	int result = dialog.run();

	switch (result) {
		case (Gtk::RESPONSE_OK):
			{
				MGNL_PRINTF(tag, LOG_LEVEL_TRACE, "Open clicked.\n");
				std::string pathname = dialog.get_filename();
				set_image_path(pathname.c_str());
				MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "File selected: %s\n", pathname.c_str());
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

void MagnoliaSetImagePathWindow::set_image_path(const char *path)
{
	memcpy(image_path_, path, sizeof(image_path_));
	p_entry_->set_text(image_path_);
	MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "%s this = %p\n", image_path_, this);
}

void MagnoliaSetImagePathWindow::set_parent_window(Gtk::Window *parent_window)
{
	parent_window_ = parent_window;
}

Gtk::Window* MagnoliaSetImagePathWindow::get_parent_window()
{
	return parent_window_;
}

char* MagnoliaSetImagePathWindow::get_image_path()
{
	return image_path_;
}
