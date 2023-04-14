#include "MainWindow.h"
#include <excpetion>

Main_Window::Main_Window()
: Gtk::Window()
{
    set_default_size(400, 400);
    set_title("Screen Recorder");
    set_border_width(20);
    layout.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    add(layout);
}

Main_Window::~Main_Window()
{
    // simply clear the observers vector since its component will be deleted later
    observers.clear();
}

void Main_Window::set_buttons(std::vector<Gtk::Button>& btns)
{
    // main window can tell the state and which buttons
    // go to which signal handler by the size of the btns vector
    while (layout.get_last_child())
    {
        layout.remove(layout.get_last_child());
    }
    if (btns.size() == 1)
    {
        layout.add(btns[0]);
        btns[0].signal_clicked().connect(
            sigc::mem_fun(*this, &Main_Window::on_stop_clicked));
    }
    else if (btns.size() == 2)
    {
        layout.add(btns[0]);
        btns[0].signal_clicked().connect(
            sigc::mem_fun(*this, &Main_Window::on_start_clicked));
        layout.add(btns[1]);
        btns[1].signal_clicked().connect(
            sigc::mem_fun(*this, &Main_Window::on_settings_clicked));
    }
    else
    {
        throw std::exception("Invalid btns vector size");
    }
}

void Main_Window::on_start_clicked()
{
    notify_observers(1);
}

void Main_Window::on_stop_clicked()
{
    notify_observers(0);
}

void Main_Window::on_settings_clicked()
{
    notify_observers(2); 
}

void Main_Window::attach(Observer& o)
{
    observers.push_back(&o);
}

void Main_Window::detach(Observer& o)
{
    int i = 0;
    while (observers[i] != &o) ++i;
    observers.erase(observers.begin() + i);
}

void Main_Window::notify_observers(int s)
{
    for (Observer* o : observers) o->update(s);
}