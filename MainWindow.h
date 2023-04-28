#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <gtkmm.h>
#include <vector>
#include "Subject.h"
#include "Observer.h"


class Main_Window : public Gtk::Window, public Subject
{
public:
    // constructor
    Main_Window();
    // destructor
    ~Main_Window();

    // set_buttons responsible for showing the correct buttons based on state
    void set_buttons(std::vector<Gtk::Button*>& btns);
    
    // attach detach and notify_observers overridden from Subject
    void attach(Observer& o) override;
    void detach(Observer& o) override;
    void notify_observers(int s) override;
protected:

    // methods for signal handlers for buttons
    void on_start_clicked();
    void on_stop_clicked();
    void on_settings_clicked();

private:
    // observers vector
    std::vector<Observer*> observers;

    // widget components for thw window
    Gtk::Box layout;
    Gtk::Button start_btn;
    Gtk::Button stop_btn;
    Gtk::Button settings_btn;
};

#endif