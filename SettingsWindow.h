#ifndef _SETTINGS_WINDOW_H_
#define _SETTINGS_WINDOW_H_

#include <gtkmm.h>
#include <vector>
#include <string>
#include "Subject.h"
#include "Observer.h"
#include "SettingsFileManager.h"

class Settings_Window : public Gtk::Window, public Subject
{
public:
    Settings_Window();

    ~Settings_Window();

    std::vector<std::string>& get_settings();

    void retrieve_settings();

    void apply_settings();

    void attach(Observer& o) override; 

    void detach(Observer& o) override;

    void notify_observers(int s) override;

protected:
    void format_changed();

    void bitrate_changed();

    void framerate_changed();

private:
    std::vector<Observer*> observers;
    std::vector<std::string> settings_list;
    Settings_File_Manager* settings_file_manager;
    Gtk::Button save_btn;
    Glib::RefPtr<Gtk::StringList> format_list;
    Glib::RefPtr<Gtk::StringList> bitrate_list;
    Glib::RefPtr<Gtk::StringList> framerate_list;
    Gtk::Label format_lbl;
    Gtk::Label bitrate_lbl;
    Gtk::Label framerate_lbl;
    Gtk::DropDown format_dd;
    Gtk::DropDown bitrate_dd;
    Gtk::DropDown framerate_dd;
    Gtk::Box layout;
};

#endif