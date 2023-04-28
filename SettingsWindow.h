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
    // Constructor
    Settings_Window();

    // Destructor
    ~Settings_Window();

    // get_settings returns the settings list
    std::vector<std::string>& get_settings();

    // retrieve settings gets the settings from the file manager
    void retrieve_settings();

    // apply settings saves the output to the external file
    void apply_settings();

    // attach
    void attach(Observer& o) override; 

    // detach
    void detach(Observer& o) override;

    // notify_observers
    void notify_observers(int s) override;

protected:
    // signal handlers for the dropdown menus
    void format_changed();

    void bitrate_changed();

    void framerate_changed();

private:
    // observers vector
    std::vector<Observer*> observers;
    // settings list
    std::vector<std::string> settings_list;
    // file manager for saving and loading settings
    Settings_File_Manager* settings_file_manager;
    
    // gtk widgets for the window
    Gtk::Button save_btn;
    Gtk::Label format_lbl;
    Gtk::Label bitrate_lbl;
    Gtk::Label framerate_lbl;
    Gtk::ComboBoxText format_dd;
    Gtk::ComboBoxText bitrate_dd;
    Gtk::ComboBoxText framerate_dd;
    Gtk::Box layout;
    Gtk::Box format_box;
    Gtk::Box bitrate_box;
    Gtk::Box framerate_box;
};

#endif