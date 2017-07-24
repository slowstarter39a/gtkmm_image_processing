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
#include "../lib/image_processing_main.h"

using namespace std;

void do_thread_work(MagnoliaImageWindow *image_window)
{
	int i = 0;
	while(i < 3) {
		std::cout<<"i = " <<i<<endl;
		sleep(1);
		i++;
	}

	Glib::RefPtr<Gdk::Pixbuf> image_read_buf =  image_window->get_current_image_pixbuf();
	Gdk::Pixbuf & image = *image_read_buf.operator->(); // just for convenience

	if (image.get_colorspace() != Gdk::COLORSPACE_RGB ) return;
	if (image.get_bits_per_sample() != 8 ) return;


	void *handle;
	handle = dlopen("image_processing_lib.so", RTLD_LAZY);
	if(!handle)
	{
		std::cout<<"Open Library failed"<<endl;
		std::cout<<dlerror()<<endl;
		return;
	}
	init_t* init_myFunc = (init_t *)dlsym(handle, "lib_test");
	if(dlerror() != NULL)
	{
		std::cout<<"Open Library function lib_test failed"<<endl;
		return; 
	}
	init_myFunc(1)->lib_func();


	int iW = image.get_width();
	int iH = image.get_height();
	guchar * pPixels = image.get_pixels();
	int iNChannels = image.get_n_channels();

	int rowstride = image.get_rowstride();
	std::cout<<"rowstride"<<rowstride<<endl;
	std::cout<<"iNChannels"<<iNChannels<<endl;

	for (int iY = 0; iY < iH; iY++) {
		for (int iX = 0; iX < iW; iX++) {
			int iOffset = iY*image.get_rowstride() + iX*iNChannels;
			pPixels[iOffset] = 255 - pPixels[iOffset];
			pPixels[iOffset+1] = 255 - pPixels[iOffset+1];
			pPixels[iOffset+2] = 255 - pPixels[iOffset+2];
		}
	}


	//MagnoliaImageWindow *image_window = magnolia_parent->get_current_image_window();

	image_window->queue_draw(); // redraw after modify
	image_window->present(); 


	return;
	//return nullptr;

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
	std::cout<<"p_button_test_"<<endl; 
}

void MagnoliaControlWindow::on_button_inverse_clicked()
{
	std::cout<<"on_button_inverse_clicked"<<endl;

	if(worker_thread_)
	{
		std::cout<<"Can't start a worker thread while another one is running."<<std::endl;
		return;
	}
	
	MagnoliaMainWindow *magnolia_parent = dynamic_cast<MagnoliaMainWindow*>(parent_window_);
	MagnoliaImageWindow *image_window = magnolia_parent->get_current_image_window();

	worker_thread_ = new std::thread(do_thread_work, image_window);
	if(!worker_thread_)
	{
		std::cout<<"creating worker_thread_ failed"<<endl;
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
	std::cout<<"magnolia_parent "<<magnolia_parent<<endl;
	std::cout<<"get_current_image_window() "<<magnolia_parent->get_current_image_window()<<endl;
	image_read_buf = magnolia_parent->get_current_image_window()->get_current_image_pixbuf();
	return image_read_buf;

}
