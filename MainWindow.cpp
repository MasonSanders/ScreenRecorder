#include "MainWindow.h"
#include <stdexcept>
#include <iostream>

// Constructor
Main_Window::Main_Window()
: Gtk::Window()
{
    // set some parameters for the window
    set_default_size(400, 400);
    set_title("Screen Recorder");
    set_border_width(20);
    layout.set_orientation(Gtk::ORIENTATION_HORIZONTAL);

    // add the layout and buttons to the application
    add(layout);
    start_btn.set_label("Start Recording");
    stop_btn.set_label("Stop Recording");
    settings_btn.set_label("Settings");
    start_btn.signal_clicked().connect(
        sigc::mem_fun(*this, &Main_Window::on_start_clicked));
    settings_btn.signal_clicked().connect(
        sigc::mem_fun(*this, &Main_Window::on_settings_clicked));
    stop_btn.signal_clicked().connect(
        sigc::mem_fun(*this, &Main_Window::on_stop_clicked));
    layout.show();
    layout.pack_start(start_btn);
    layout.pack_start(stop_btn);
    layout.pack_start(settings_btn);
}

// destructor
Main_Window::~Main_Window()
{
    // simply clear the observers vector since its component will be deleted later
    observers.clear();
}

// set_buttons
void Main_Window::set_buttons(std::vector<Gtk::Button*>& btns)
{
    // main window can tell the state and which buttons
    // go to which signal handler by the size of the btns vector
    // however the btns vector is deprecated, as are the button factory and the specialized buttons
    // regular gtk::buttons work just fine as they just need to be connected to a signal handler
    // and passing buttons in created problems with old buttons being deleted as there was no good way
    // to remove them from the layout if they weren't stored as member variables.
    // realistically, the main window could have methods show_not_recording_btns and show_recording_btns to replace this method entirely.
    if (btns.size() == 1)
    {
        // show the stop button
        start_btn.hide();
        settings_btn.hide();
        stop_btn.show();
    }
    else if (btns.size() == 2)
    {   
        // show the start and settings buttons
        stop_btn.hide();
        start_btn.show();
        settings_btn.show();
    }
    else
    {
        throw std::runtime_error("Invalid btns vector size");
    }
}

// on_start_clicked
void Main_Window::on_start_clicked()
{
    notify_observers(1);
}

// on_stop_clicked
void Main_Window::on_stop_clicked()
{
    notify_observers(0);
}

// on_settings_clicked
void Main_Window::on_settings_clicked()
{
    notify_observers(2); 
}

// attach
void Main_Window::attach(Observer& o)
{
    observers.push_back(&o);
}

// detach
void Main_Window::detach(Observer& o)
{
    int i = 0;
    while (observers[i] != &o) ++i;
    observers.erase(observers.begin() + i);
}

// notify_observers
void Main_Window::notify_observers(int s)
{
    for (Observer* o : observers) o->update(s);
}