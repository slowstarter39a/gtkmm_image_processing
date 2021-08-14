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
#include "magnolia_control_window.h"
#include "magnolia_image_window.h"
#include "magnolia_main_window.h"
#include "image_processing_main.h"
#include "magnolia_common_data.h"
#include "magnolia_logger.h"
#include <vector>

using namespace std;
static const char *tag = __FILE__;

void do_thread_work(MagnoliaImageWindow *image_window, int lib_type, magnolia_cmd_param_type *cmd, int result_image_count)
{
	int result = MAGNOLIA_FAILURE;

	void *handle;
	handle = dlopen("image_processing_lib.so", RTLD_LAZY);
	if (!handle) {
		MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "Open Library failed. %s\n", dlerror());
		return;
	}

	Glib::RefPtr<Gdk::Pixbuf> src_image_pixbuf =  image_window->get_src_image_pixbuf();
	Gdk::Pixbuf &src_pixbuf= *src_image_pixbuf.operator->(); // just for convenience

	Glib::ustring src_label_text = image_window->get_src_image_label_text();
	vector<pixbuf_label> src_img = {{&src_pixbuf, &src_label_text}};

	Glib::RefPtr<Gdk::Pixbuf> *dst_image_pixbuf = new Glib::RefPtr<Gdk::Pixbuf>[result_image_count];
	Glib::ustring *dst_label_text = new Glib::ustring[result_image_count];
	vector<pixbuf_label> dst_img;

	for (int i = 0; i < result_image_count; i++) {
		dst_image_pixbuf[i] = Gdk::Pixbuf::create(src_pixbuf.get_colorspace(), src_pixbuf.get_has_alpha(), src_pixbuf.get_bits_per_sample(),
				src_pixbuf.get_width(), src_pixbuf.get_height());
		Gdk::Pixbuf &dst_pixbuf = *(dst_image_pixbuf[i]).operator->();
		dst_img.push_back({&dst_pixbuf, &dst_label_text[i]});
	}

	image_processing_handler_t *fnImageProcessing = (image_processing_handler_t*)dlsym(handle, "ImageProcessingDispatcher");
	if (dlerror() != NULL) {
		MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "Open Library function ImageProcessingDispatcher failed\n");
		goto end;
	}

	result = fnImageProcessing(lib_type, cmd, src_img, dst_img);

	if (!result) {
		for (int i = 0; i < result_image_count; i++) {
			image_window->show_dst_image(dst_image_pixbuf[i], dst_label_text[i], i);
		}
	}
	else {
		MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "result of fnImageProcessing = %d\n", result);
	}

end:
	dlclose(handle);
	delete []dst_image_pixbuf;
	delete []dst_label_text;
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
	magnolia_control_ref_glade_->get_widget("id_button_color_image_to_gray_image", p_button_color_image_to_gray_image_);

	p_button_test_->signal_clicked().connect(sigc::mem_fun(*this, &MagnoliaControlWindow::on_test));
	p_button_inverse_->signal_clicked().connect(sigc::mem_fun(*this, &MagnoliaControlWindow::on_button_inverse_clicked)); 
	p_button_color_image_to_gray_image_->signal_clicked().connect(sigc::mem_fun(*this, &MagnoliaControlWindow::on_button_color_image_to_gray_image_clicked)); 

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

void MagnoliaControlWindow::run_thread(magnolia_cmd_param_type &cmd, int result_image_count)
{
	std::lock_guard<std::mutex> guard(control_mutex);

	if (worker_thread_) {
		MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "Can't start a worker thread while another one is running.\n");
		return;
	}
	
	MagnoliaMainWindow *magnolia_parent = dynamic_cast<MagnoliaMainWindow*>(parent_window_);
	MagnoliaImageWindow *image_window = magnolia_parent->get_current_image_window();

	int lib_type = magnolia_parent->get_magnolia_xml_struct()->get_lib_type();

	worker_thread_ = new std::thread(do_thread_work, image_window, lib_type, &cmd, result_image_count);
	if (!worker_thread_) {
		MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "creating worker_thread_ failed\n");
		return;
	}
	worker_thread_->join();

	delete worker_thread_;
	worker_thread_ = NULL;
}

void MagnoliaControlWindow::on_test()
{
	MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "p_button_test\n");
}

void MagnoliaControlWindow::on_button_color_image_to_gray_image_clicked()
{
	MGNL_PRINTF(tag, LOG_LEVEL_TRACE, "on_button_color_image_to_gray_image_clicked\n");

	magnolia_cmd_param_type cmd;
	cmd.mag_cmd = MAGNOLIA_CMD_COLOR_IMAGE_TO_GRAY_IMAGE;
	int result_image_count = 3;

	run_thread(cmd, result_image_count);
}

void MagnoliaControlWindow::on_button_inverse_clicked()
{
	MGNL_PRINTF(tag, LOG_LEVEL_TRACE, "on_button_inverse_clicked\n");

	magnolia_cmd_param_type cmd;
	cmd.mag_cmd = MAGNOLIA_CMD_INVERSION;
	int result_image_count = 1;

	run_thread(cmd, result_image_count);
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

