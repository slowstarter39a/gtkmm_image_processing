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

#include <dlfcn.h>
#include <iostream>
#include "magnolia_control_window.h"
#include "magnolia_image_window.h"
#include "magnolia_main_window.h"
#include "image_processing_main.h"
#include "magnolia_common_data.h"
#include "magnolia_logger.h"

using namespace std;
static const char *tag = __FILE__;

void do_thread_work(MagnoliaImageWindow *image_window, int lib_type, magnolia_cmd_type *cmd)
{
	int result = FAILURE;

	Glib::RefPtr<Gdk::Pixbuf> image_src_buf =  image_window->get_src_image_pixbuf();
	Gdk::Pixbuf &src_img= *image_src_buf.operator->(); // just for convenience

	int dst_img_width = src_img.get_width();
	int dst_img_height = src_img.get_height();
	Glib::RefPtr<Gdk::Pixbuf> image_dst_buf = Gdk::Pixbuf::create(src_img.get_colorspace(), src_img.get_has_alpha(), src_img.get_bits_per_sample(),
			dst_img_width, dst_img_height);
	Gdk::Pixbuf &dst_img = *image_dst_buf.operator->();

	void *handle;
	handle = dlopen("image_processing_lib.so", RTLD_LAZY);
	if (!handle) {
		MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "Open Library failed. %s\n", dlerror());
		return;
	}
	image_processing_handler_t *fnImageProcessing = (image_processing_handler_t*)dlsym(handle, "ImageProcessingDispatcher");
	if (dlerror() != NULL) {
		MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "Open Library function ImageProcessingDispatcher failed\n");
		return;
	}
	result = fnImageProcessing(lib_type, cmd, src_img, dst_img);

	if (!result) {
		image_window->show_dst_image(image_dst_buf, dst_img_width, dst_img_height);
	}
	else {
		MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "result of fnImageProcessing = %d\n", result);
	}
}

MagnoliaControlWindow::MagnoliaControlWindow()
{ 
	worker_thread_ = NULL;
}

MagnoliaControlWindow::~MagnoliaControlWindow()
{

}

MagnoliaControlWindow::MagnoliaControlWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
	: Gtk::Window(cobject),
	magnolia_control_ref_glade_(refGlade)
{
	magnolia_control_ref_glade_->get_widget("id_button_test", p_button_test_);
	magnolia_control_ref_glade_->get_widget("id_button_inverse", p_button_inverse_);

	p_button_test_->signal_clicked().connect(sigc::mem_fun(*this, &MagnoliaControlWindow::on_test));
	p_button_inverse_->signal_clicked().connect(sigc::mem_fun(*this, &MagnoliaControlWindow::on_button_inverse_clicked)); 

	show_all_children(); 
	worker_thread_ = NULL;
} 

void MagnoliaControlWindow::set_parent_window(Gtk::Window *parent_window)
{
	parent_window_ = parent_window;
}

Gtk::Window* MagnoliaControlWindow::get_parent_window()
{
	return parent_window_;
}

void MagnoliaControlWindow::on_test()
{
	MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "p_button_test\n");
}

void MagnoliaControlWindow::on_button_inverse_clicked()
{
	MGNL_PRINTF(tag, LOG_LEVEL_TRACE, "on_button_inverse_clicked\n");

	if (worker_thread_) {
		MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "Can't start a worker thread while another one is running.\n");
		return;
	}
	
	MagnoliaMainWindow *magnolia_parent = dynamic_cast<MagnoliaMainWindow*>(parent_window_);
	MagnoliaImageWindow *image_window = magnolia_parent->get_current_image_window();

	magnolia_cmd_type cmd;
	cmd.mag_cmd = MAGNOLIA_CMD_INVERSE;

	int lib_type = magnolia_parent->get_magnolia_xml_struct()->get_lib_type();

	worker_thread_ = new std::thread(do_thread_work, image_window, lib_type, &cmd);
	if (!worker_thread_) {
		MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "creating worker_thread_ failed\n");
		return;
	}
	worker_thread_->join();

	delete worker_thread_;
	worker_thread_ = NULL;
}

Glib::RefPtr<Gdk::Pixbuf> MagnoliaControlWindow::get_current_image_buf()
{
	MagnoliaMainWindow *magnolia_parent = dynamic_cast<MagnoliaMainWindow*>(parent_window_);

	Glib::RefPtr<Gdk::Pixbuf> image_read_buf;
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "magnolia_parent = %p\n", magnolia_parent);
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "get_current_image_window() = %p\n", magnolia_parent->get_current_image_window());
	image_read_buf = magnolia_parent->get_current_image_window()->get_src_image_pixbuf();
	return image_read_buf;
}

