#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <gtkmm.h>
#include <vector>
#include "Subject.h"
#include "Observer.h"


class Main_Window : public Gtk::Window, public Subject
{
public:
    Main_Window();
    ~Main_Window();
    void set_buttons(std::vector<Gtk::Button>& btns);
    void attach(Observer& o) override;
    void detach(Observer& o) override;
    void notify_observers(int s) override;
protected:
    void on_start_clicked();
    void on_stop_clicked();
    void on_settings_clicked();

private:
    // does this need to be a vector of observers or observer references?
    std::vector<Observer*> observers;
    Gtk::Box layout;
}

#endif