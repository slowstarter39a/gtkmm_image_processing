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

#include <iostream>
#include "magnolia_image_window.h"
#include "magnolia_main_window.h"

using namespace std;

MagnoliaImageWindow::MagnoliaImageWindow(Gtk::Window *parent_window)
{
	set_border_width(10);
	set_default_size(800, 800);

	fixed_ = NULL;

	show_all_children(); 
	parent_window_ = parent_window;
}
MagnoliaImageWindow::MagnoliaImageWindow(Gtk::Window *parent_window, std::string filename)
{
	std::cout<<filename<<std::endl;
	set_border_width(10);
	set_default_size(800, 800);

	ImageListStruct *img_list = new ImageListStruct;
	img_list->frame = new Gtk::Frame;
	img_list->eventbox = new Gtk::EventBox;
	img_list->image = new Gtk::Image;
	//std::map<int, ImageListStruct*> image_list_;
	fixed_ = new Gtk::Fixed;

	if(!img_list || !img_list->frame || !img_list->eventbox || !img_list->image || !fixed_)
		return;

	add(*fixed_);

	img_list->image->set(filename);

	Gdk::RGBA color;
	color.set_rgba(0.9294, 0.9921, 1.0,1.0);
	img_list->frame->override_background_color(color, Gtk::STATE_FLAG_NORMAL);
	img_list->frame->add(*(img_list->eventbox));
	img_list->eventbox->add(*(img_list->image));
	img_list->image_id = image_cnt;
	fixed_->add(*(img_list->frame));

	img_list->eventbox->set_events(Gdk::BUTTON_PRESS_MASK);
	img_list->eventbox->signal_button_press_event().connect(sigc::bind<int>
			(sigc::mem_fun(*this, &MagnoliaImageWindow::on_eventbox_button_press), img_list->image_id));


	image_list_[image_cnt++] = img_list;
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

	for(iter = image_list_.begin(); iter != image_list_.end(); iter++){
		img_list = iter->second;

		std::stringstream ss;
		ss << "Deleting List '" << iter->first <<"'";
		std::cout << ss.str() << std::endl;

		delete img_list->frame;
		delete img_list->eventbox;
		delete img_list->image;
		delete img_list;
	}

}

void MagnoliaImageWindow::on_show()
{
	Gtk::Window::on_show();

	if (image_list_.size()) {
		current_img_list_struct_->frame->set_size_request(current_img_list_struct_->image->get_width() + 10, 
				current_img_list_struct_->image->get_height() + 10);
//		image_list_[0]->frame->set_size_request(image_list_[0]->image->get_width()+10,
//				image_list_[0]->image->get_height()+10);
	}

}


bool MagnoliaImageWindow::on_focus_in_event(GdkEventFocus* focus_event)
{
	std::cout<<"on_focus_in_event"<<endl;

	MagnoliaMainWindow *magnolia_parent = dynamic_cast<MagnoliaMainWindow*>(parent_window_);
	std::cout<<"magnolia_parent "<<magnolia_parent<<endl;
	magnolia_parent->set_current_image_window(this); 

	return 1;
}

bool MagnoliaImageWindow::on_eventbox_button_press(GdkEventButton *, int image_id) 
{
	std::cout<<"on_eventbox_button_press "<<image_id<<endl;

	std::map<int, ImageListStruct*>::iterator iter;
	ImageListStruct *img_list = NULL;
	Gdk::RGBA color;

	for(iter = image_list_.begin(); iter != image_list_.end(); iter++){
		img_list = iter->second;

		if(img_list->image_id == image_id) {
			color.set_rgba(0.9294, 0.9921, 1.0, 3.0);
			current_img_list_struct_ = img_list;
		}
		else {
			color.set_rgba(0.8588, 0.8588, 0.8588, 1.0);
		}

		img_list->frame->override_background_color(color, Gtk::STATE_FLAG_NORMAL); 
	} 

	return TRUE;
}

Glib::RefPtr<Gdk::Pixbuf> MagnoliaImageWindow::get_src_image_pixbuf()
{
	return current_img_list_struct_->image->get_pixbuf();
}

void MagnoliaImageWindow::show_dst_image(Glib::RefPtr<Gdk::Pixbuf> &dst_buf)
{ 
	ImageListStruct *img_list = new ImageListStruct;
	img_list->frame = new Gtk::Frame;
	img_list->eventbox = new Gtk::EventBox;
	img_list->image= new Gtk::Image;
	img_list->image->set(dst_buf);
	//img_list->image= new Gtk::Image;

	if(!img_list || !img_list->frame || !img_list->eventbox || !img_list->image)
		return;

	Gdk::RGBA color;
	color.set_rgba(0.8588, 0.8588, 0.8588, 1.0);
	img_list->frame->override_background_color(color, Gtk::STATE_FLAG_NORMAL);
	img_list->frame->add(*(img_list->eventbox));
	img_list->eventbox->add(*(img_list->image));
	img_list->image_id = image_cnt;
	fixed_->add(*(img_list->frame));

	img_list->eventbox->set_events(Gdk::BUTTON_PRESS_MASK);
	img_list->eventbox->signal_button_press_event().connect(sigc::bind<int>
			(sigc::mem_fun(*this, &MagnoliaImageWindow::on_eventbox_button_press), img_list->image_id));


	image_list_[image_cnt++] = img_list;
	current_img_list_struct_ = img_list;

	show_all_children();
}
