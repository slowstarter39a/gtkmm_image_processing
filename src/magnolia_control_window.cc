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
#include "magnolia_main_window.h"
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
    magnolia_control_ref_glade_->get_widget("id_button_inverse", p_button_inverse_);

    p_button_test_->signal_clicked().connect(sigc::mem_fun(*this, &MagnoliaControlWindow::on_test));
    p_button_inverse_->signal_clicked().connect(sigc::mem_fun(*this, &MagnoliaControlWindow::on_button_inverse_clicked)); 

    show_all_children(); 
} 

void MagnoliaControlWindow::on_test()
{
    std::cout<<"p_button_test_"<<endl;
    Gtk::Window *parent = get_transient_for();
    MagnoliaMainWindow *magnolia_parent = dynamic_cast<MagnoliaMainWindow*>(parent);
    Glib::RefPtr<Gdk::Pixbuf> image_read_buf;

}

void MagnoliaControlWindow::on_button_inverse_clicked()
{
    //Todo Should modify this function
    std::cout<<"on_button_inverse_clicked"<<endl;
    Gtk::Window *parent = get_transient_for();
    MagnoliaMainWindow *magnolia_parent = dynamic_cast<MagnoliaMainWindow*>(parent);
    Glib::RefPtr<Gdk::Pixbuf> image_read_buf = get_current_image_buf();
    Gdk::Pixbuf & image = *image_read_buf.operator->(); // just for convenience

    if ( ! image.get_colorspace() == Gdk::COLORSPACE_RGB ) return;
    if ( ! image.get_bits_per_sample() == 8 ) return;


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


    MagnoliaImageWindow *image_window = magnolia_parent->get_current_image_window();

    image_window->queue_draw(); // redraw after modify
    image_window->present();
}

Glib::RefPtr<Gdk::Pixbuf> MagnoliaControlWindow::get_current_image_buf()
{
    Gtk::Window *parent = get_transient_for();
    MagnoliaMainWindow *magnolia_parent = dynamic_cast<MagnoliaMainWindow*>(parent);

    Glib::RefPtr<Gdk::Pixbuf> image_read_buf;
    std::cout<<"magnolia_parent "<<magnolia_parent<<endl;
    std::cout<<"get_current_image_window() "<<magnolia_parent->get_current_image_window()<<endl;
    image_read_buf = magnolia_parent->get_current_image_window()->get_current_image_pixbuf();
    return image_read_buf;

}
