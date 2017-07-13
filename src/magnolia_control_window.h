/*
 * =====================================================================================
 *
 *       Filename:  magnolia_control_window.h
 *
 *    Description:  
 *      This file is a magnolia control window header file
 *
 * =====================================================================================
 */

#ifndef _MAGNOLIA_CONTROL_WINDOW_H
#define _MAGNOLIA_CONTROL_WINDOW_H 

#include <gtkmm.h>

class MagnoliaControlWindow : public Gtk::Window
{
    public:
        MagnoliaControlWindow();
        MagnoliaControlWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
        virtual ~MagnoliaControlWindow();
        void on_test();
        void on_button_inverse_clicked();
        Glib::RefPtr<Gdk::Pixbuf> get_current_image_buf();

    protected:
    private:
        Glib::RefPtr<Gtk::Builder> magnolia_control_ref_glade_;
        Gtk::Button* p_button_test_ ;
        Gtk::Button* p_button_inverse_;
};

#endif
