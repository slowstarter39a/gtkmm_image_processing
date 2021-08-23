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

ImageListStruct* was_dst_image_already_created(magnolia_param_type *cmd);
void store_dst_image_struct(magnolia_param_type *cmd, ImageListStruct *image);

void do_thread_work(MagnoliaImageWindow *image_window, int lib_type, magnolia_param_type *cmd)
{
	int result = MAGNOLIA_FAILURE;

	void *handle;
	handle = dlopen("image_processing_lib.so", RTLD_LAZY);
	if (!handle) {
		MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "Open Library failed. %s\n", dlerror());
		return;
	}

	Glib::RefPtr<Gdk::Pixbuf> src_image_pixbuf =  image_window->get_current_image_pixbuf();
	Gdk::Pixbuf &src_pixbuf= *src_image_pixbuf.operator->(); // just for convenience

	Glib::ustring src_label_text = image_window->get_current_image_label_text();
	vector<pixbuf_label> src_img = {{&src_pixbuf, &src_label_text}};

	Glib::RefPtr<Gdk::Pixbuf> *dst_image_pixbuf = new Glib::RefPtr<Gdk::Pixbuf>[cmd->result_image_count];
	Glib::ustring *dst_label_text = new Glib::ustring[cmd->result_image_count];
	vector<pixbuf_label> dst_img;

	for (int i = 0; i < cmd->result_image_count; i++) {
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
		for (int i = 0; i < cmd->result_image_count; i++) {
			ImageListStruct *image = was_dst_image_already_created(cmd);
			if (image) {
				image_window->update_dst_image(image, dst_image_pixbuf[i], dst_label_text[i], i);
			}
			else {
				image = image_window->show_dst_image(dst_image_pixbuf[i], dst_label_text[i], i);
				store_dst_image_struct(cmd, image);
			}
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

MagnoliaControlWindow::~MagnoliaControlWindow()
{

}

MagnoliaControlWindow::MagnoliaControlWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
	: Gtk::Window(cobject),
	magnolia_control_ref_glade_(refGlade)
{

	worker_thread_ = nullptr;
	dst_image_binarization_ = nullptr;

	magnolia_control_ref_glade_->get_widget("id_button_color_image_to_gray_image", p_button_color_image_to_gray_image_);
	magnolia_control_ref_glade_->get_widget("id_button_inverse", p_button_inverse_);
	magnolia_control_ref_glade_->get_widget("id_checkbutton_image_binarization", p_checkbutton_image_binarization_);
	magnolia_control_ref_glade_->get_widget("id_scale_image_binarization", p_scale_image_binarization_);
	p_scale_image_binarization_->set_range(0, 255);
	p_scale_image_binarization_->set_value(128);
	p_scale_image_binarization_->set_sensitive(false);
	magnolia_control_ref_glade_->get_widget("id_button_histogram", p_button_histogram_);
	magnolia_control_ref_glade_->get_widget("id_button_close", p_button_close_);

	p_button_color_image_to_gray_image_->signal_clicked().connect(sigc::mem_fun(*this, &MagnoliaControlWindow::on_button_color_image_to_gray_image_clicked));
	p_button_inverse_->signal_clicked().connect(sigc::mem_fun(*this, &MagnoliaControlWindow::on_button_inverse_clicked));
	p_checkbutton_image_binarization_->signal_clicked().connect(sigc::mem_fun(*this, &MagnoliaControlWindow::on_checkbutton_image_binarization_clicked));
	p_scale_image_binarization_->signal_value_changed().connect(sigc::mem_fun(*this, &MagnoliaControlWindow::on_scale_image_binarization_value_changed));

	p_button_histogram_->signal_clicked().connect(sigc::mem_fun(*this, &MagnoliaControlWindow::on_button_histogram_clicked));
	p_button_close_->signal_clicked().connect(sigc::mem_fun(*this, &MagnoliaControlWindow::on_button_close_clicked));

	show_all_children();
}

void MagnoliaControlWindow::set_parent_window(Gtk::Window *parent_window)
{
	parent_window_ = parent_window;
}

Gtk::Window* MagnoliaControlWindow::get_parent_window()
{
	return parent_window_;
}

void MagnoliaControlWindow::run_thread(magnolia_param_type &cmd)
{
	std::lock_guard<std::mutex> guard(control_mutex);

	if (worker_thread_) {
		MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "Can't start a worker thread while another one is running.\n");
		return;
	}

	MagnoliaMainWindow *magnolia_parent = dynamic_cast<MagnoliaMainWindow*>(parent_window_);
	MagnoliaImageWindow *image_window = magnolia_parent->get_current_image_window();

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

void MagnoliaControlWindow::on_button_color_image_to_gray_image_clicked()
{
	MGNL_PRINTF(tag, LOG_LEVEL_TRACE, "\n");

	magnolia_param_type cmd;
	memset(&cmd, 0x0, sizeof(magnolia_param_type));

	cmd.mag_cmd = MAGNOLIA_CMD_COLOR_IMAGE_TO_GRAY_IMAGE;
	cmd.result_image_count = 3;

	run_thread(cmd);
}

void MagnoliaControlWindow::on_button_inverse_clicked()
{
	MGNL_PRINTF(tag, LOG_LEVEL_TRACE, "\n");

	magnolia_param_type cmd;
	memset(&cmd, 0x0, sizeof(magnolia_param_type));

	cmd.mag_cmd = MAGNOLIA_CMD_INVERSION;
	cmd.result_image_count = 1;

	run_thread(cmd);
}

void MagnoliaControlWindow::on_checkbutton_image_binarization_clicked()
{
	MGNL_PRINTF(tag, LOG_LEVEL_TRACE, "get_active = %d\n", p_checkbutton_image_binarization_->get_active());

	if (p_checkbutton_image_binarization_->get_active()) {
		p_scale_image_binarization_->set_sensitive(true);
		on_scale_image_binarization_value_changed();
	}
	else {
		p_scale_image_binarization_->set_sensitive(false);
		dst_image_binarization_ = nullptr;
	}
}

void MagnoliaControlWindow::on_scale_image_binarization_value_changed()
{
	MGNL_PRINTF(tag, LOG_LEVEL_TRACE, "get_value = %d\n", int (p_scale_image_binarization_->get_value()));

	magnolia_param_type cmd;
	memset(&cmd, 0x0, sizeof(magnolia_param_type));

	cmd.mag_cmd = MAGNOLIA_CMD_IMAGE_BINARIZATION;
	cmd.param.img_binarization.value = int(p_scale_image_binarization_->get_value());

	cmd.param.img_binarization.dst_image = (void**)&dst_image_binarization_;
	cmd.result_image_count = 1;

	run_thread(cmd);
}

void MagnoliaControlWindow::on_button_histogram_clicked()
{
	MGNL_PRINTF(tag, LOG_LEVEL_TRACE, "on_button_histogram_clicked\n");

	magnolia_param_type cmd;
	memset(&cmd, 0x0, sizeof(magnolia_param_type));

	cmd.mag_cmd = MAGNOLIA_CMD_HISTOGRAM;
	cmd.result_image_count = 1;

	run_thread(cmd);
}

//call set_active(false); for all checkboxes in control window when control window focus out.
bool MagnoliaControlWindow::on_focus_out_event(GdkEventFocus* focus_event)
{
	MGNL_PRINTF(tag, LOG_LEVEL_TRACE, "%s\n", __FUNCTION__);
	p_checkbutton_image_binarization_->set_active(false);

	return 1;
}

void MagnoliaControlWindow::on_button_close_clicked()
{
	hide();
}

Glib::RefPtr<Gdk::Pixbuf> MagnoliaControlWindow::get_current_image_buf()
{
	MagnoliaMainWindow *magnolia_parent = dynamic_cast<MagnoliaMainWindow*>(parent_window_);

	Glib::RefPtr<Gdk::Pixbuf> image_read_buf;
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "magnolia_parent = %p\n", magnolia_parent);
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "get_current_image_window() = %p\n", magnolia_parent->get_current_image_window());
	image_read_buf = magnolia_parent->get_current_image_window()->get_current_image_pixbuf();
	return image_read_buf;
}

ImageListStruct* was_dst_image_already_created(magnolia_param_type *cmd)
{
	MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "cmd->mag_cmd = %d\n", cmd->mag_cmd);
	if (cmd->mag_cmd == MAGNOLIA_CMD_IMAGE_BINARIZATION) {
		MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "cmd->param.img_binarization.dst_imag = %p\n", cmd->param.img_binarization.dst_image);
		return (ImageListStruct*)*(cmd->param.img_binarization.dst_image);
	}

	return nullptr;
}

void store_dst_image_struct(magnolia_param_type *cmd, ImageListStruct *image)
{
	if (cmd->mag_cmd == MAGNOLIA_CMD_IMAGE_BINARIZATION) {
		MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "cmd->param.img_binarization.dst_image = %d\n", cmd->param.img_binarization.dst_image);
		*(cmd->param.img_binarization.dst_image) = image;
	}
}
