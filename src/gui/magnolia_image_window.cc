/*
 * =====================================================================================
 *
 *       Filename:  magnolia_image_window.cc
 *
 *    Description:  
 *      This file is a magnolia image window source file
 *
 * =====================================================================================
 */

#include "magnolia_image_window.h"
#include "magnolia_main_window.h"
#include "magnolia_logger.h"

using namespace std;
static const char *tag = __FILE__;

MagnoliaImageWindow::MagnoliaImageWindow(Gtk::Window *parent_window)
{
	set_border_width(10);
	set_default_size(800, 800);

	fixed_ = NULL;
	memset(&mouse_button_press_, 0x0, sizeof(mouse_button_press_));

	show_all_children(); 
	parent_window_ = parent_window;
}
MagnoliaImageWindow::MagnoliaImageWindow(Gtk::Window *parent_window, std::string filename)
{
	MGNL_PRINTF(tag, LOG_LEVEL_TRACE, "filename = %s\n", filename.c_str());
	set_border_width(10);
	set_default_size(800, 800);

	memset(&mouse_button_press_, 0x0, sizeof(mouse_button_press_));

	ImageListStruct *img_list = new ImageListStruct;
	img_list->frame = new Gtk::Frame;
	img_list->eventbox = new Gtk::EventBox;
	img_list->vbox = new Gtk::VBox;
	img_list->image = new Gtk::Image;
	img_list->label = new Gtk::Label;
	//std::map<int, ImageListStruct*> image_list_;
	fixed_ = new Gtk::Fixed;

	if (!img_list || !img_list->frame || !img_list->eventbox || !img_list->image || !fixed_)
		return;

	add(*fixed_);

	img_list->image->set(filename);
	img_list->label->set_text("<" + filename.substr(filename.find_last_of("/\\") + 1) + ">");
	img_list->label->override_color(Gdk::RGBA("darkblue"), Gtk::STATE_FLAG_NORMAL);

	Gdk::RGBA color;
	color.set_rgba(0.9294, 0.9921, 1.0,1.0);
	img_list->frame->override_background_color(color, Gtk::STATE_FLAG_NORMAL);
	img_list->frame->add(*(img_list->eventbox));
	img_list->eventbox->add(*(img_list->vbox));
	img_list->vbox->pack_start(*img_list->image);
	img_list->vbox->pack_start(*img_list->label);
	img_list->image_id = image_cnt_;
	fixed_->add(*(img_list->frame));

	img_list->eventbox->set_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON1_MOTION_MASK);
	img_list->eventbox->signal_button_press_event().connect(sigc::bind<int>
			(sigc::mem_fun(*this, &MagnoliaImageWindow::on_eventbox_button_press), img_list->image_id));
	img_list->eventbox->signal_motion_notify_event().connect(
			(sigc::mem_fun(*this, &MagnoliaImageWindow::on_eventbox_button_motion_notify)));

	image_list_[image_cnt_++] = img_list;
	current_img_list_struct_ = img_list;

	show_all_children();
	parent_window_ = parent_window;
}

MagnoliaImageWindow::~MagnoliaImageWindow()
{
	if (fixed_) 
		delete fixed_;

	std::map<int, ImageListStruct*>::iterator iter;
	ImageListStruct *img_list = NULL;

	for (iter = image_list_.begin(); iter != image_list_.end(); iter++){
		img_list = iter->second;

		MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "Deleting List '%d'\n", iter->first);

		delete img_list->label;
		delete img_list->image;
		delete img_list->vbox;
		delete img_list->eventbox;
		delete img_list->frame;
		delete img_list;
	}

}

void MagnoliaImageWindow::on_show()
{
	Gtk::Window::on_show();

	if (image_list_.size()) {
		current_img_list_struct_->frame->set_size_request(current_img_list_struct_->image->get_width() + 10, 
				current_img_list_struct_->image->get_height() + 30);
	}

}


bool MagnoliaImageWindow::on_focus_in_event(GdkEventFocus* focus_event)
{
	MGNL_PRINTF(tag, LOG_LEVEL_TRACE, "%s\n", __FUNCTION__);

	MagnoliaMainWindow *magnolia_parent = dynamic_cast<MagnoliaMainWindow*>(parent_window_);
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "magnolia_parent = %p\n", magnolia_parent);
	magnolia_parent->set_current_image_window(this); 

	return 1;
}

bool MagnoliaImageWindow::on_eventbox_button_press(GdkEventButton *event, int image_id) 
{
	MGNL_PRINTF(tag, LOG_LEVEL_DEBUG, "image id = %d\n", image_id);

	int frame_x_pos = 0;
	int frame_y_pos = 0;

	mouse_button_press_.x = event->x;
	mouse_button_press_.y = event->y;

	std::map<int, ImageListStruct*>::iterator iter;
	ImageListStruct *img_list = NULL;
	Gdk::RGBA color;
	Gdk::RGBA color_label;

	for (iter = image_list_.begin(); iter != image_list_.end(); iter++) {
		img_list = iter->second;

		if (img_list->image_id == image_id) {
			color.set_rgba(0.9294, 0.9921, 1.0, 3.0);
			current_img_list_struct_ = img_list;
			frame_x_pos = fixed_->child_property_x(*(current_img_list_struct_->frame));
			frame_y_pos = fixed_->child_property_y(*(current_img_list_struct_->frame)); 
			fixed_->remove(*img_list->frame);
			fixed_->put(*img_list->frame, frame_x_pos, frame_y_pos); 
			color_label = Gdk::RGBA("darkblue");
		}
		else {
			color.set_rgba(0.8588, 0.8588, 0.8588, 1.0);
			color_label = Gdk::RGBA("gray");
		}

		img_list->frame->override_background_color(color, Gtk::STATE_FLAG_NORMAL); 
		img_list->label->override_color(color_label, Gtk::STATE_FLAG_NORMAL);
		//img_list->label->override_color(Gdk::RGBA("darkblue"), Gtk::STATE_FLAG_NORMAL);
	} 


	return TRUE;
}

bool MagnoliaImageWindow::on_eventbox_button_motion_notify(GdkEventMotion* event)
{ 
	int move_x = event->x - mouse_button_press_.x;
	int move_y = event->y - mouse_button_press_.y;
	int frame_x_pos = fixed_->child_property_x(*(current_img_list_struct_->frame));
	int frame_y_pos = fixed_->child_property_y(*(current_img_list_struct_->frame)); 

	fixed_->move(*(current_img_list_struct_->frame), frame_x_pos + move_x, frame_y_pos + move_y); 

	return true;
}

Glib::RefPtr<Gdk::Pixbuf> MagnoliaImageWindow::get_src_image_pixbuf()
{
	return current_img_list_struct_->image->get_pixbuf();
}

Glib::ustring MagnoliaImageWindow::get_src_image_label_text()
{
	return current_img_list_struct_->label->get_text();
}

void MagnoliaImageWindow::show_dst_image(Glib::RefPtr<Gdk::Pixbuf> &dst_buf, Glib::ustring label_text, int result_image_count)
{ 
	int src_frame_width = current_img_list_struct_->frame->get_allocated_width();
	int src_frame_x_pos = fixed_->child_property_x(*(current_img_list_struct_->frame));
	int src_frame_y_pos = fixed_->child_property_y(*(current_img_list_struct_->frame)); 

	ImageListStruct *img_list = new ImageListStruct;
	img_list->frame = new Gtk::Frame;
	img_list->eventbox = new Gtk::EventBox;
	img_list->vbox = new Gtk::VBox;
	img_list->image= new Gtk::Image;
	img_list->label = new Gtk::Label;
	img_list->image->set(dst_buf);
	img_list->label->set_text("<" + label_text + ">");
	img_list->label->set_line_wrap(true);

	if (!img_list || !img_list->frame || !img_list->eventbox || !img_list->image)
		return;

	Gdk::RGBA color;
	color.set_rgba(0.8588, 0.8588, 0.8588, 1.0);
	img_list->frame->override_background_color(color, Gtk::STATE_FLAG_NORMAL);
	img_list->frame->add(*(img_list->eventbox));
	img_list->eventbox->add(*(img_list->vbox));
	img_list->vbox->pack_start(*img_list->image);
	img_list->vbox->pack_start(*img_list->label);
	img_list->image_id = image_cnt_;

	fixed_->put(*(img_list->frame), src_frame_x_pos + ((1 + result_image_count) *src_frame_width), src_frame_y_pos); 

	img_list->eventbox->set_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON1_MOTION_MASK);
	img_list->eventbox->signal_button_press_event().connect(sigc::bind<int>
			(sigc::mem_fun(*this, &MagnoliaImageWindow::on_eventbox_button_press), img_list->image_id)); 
	img_list->eventbox->signal_motion_notify_event().connect(
			(sigc::mem_fun(*this, &MagnoliaImageWindow::on_eventbox_button_motion_notify)));

	image_list_[image_cnt_++] = img_list;

	show_all_children();
	Gtk::Window::on_show();

	img_list->frame->set_size_request(dst_buf->get_width() + 10, dst_buf->get_height() + 30);
}
