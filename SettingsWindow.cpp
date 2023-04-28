#include "SettingsWindow.h"
#include <iostream>

// Constructor
Settings_Window::Settings_Window()
: Gtk::Window(),
  settings_list{"mp4", "4000", "30"},
  settings_file_manager(new Settings_File_Manager())
{
    // set parameters for the window
    set_default_size(400, 400);
    set_title("Edit Settings");
    // get the settings from the settings file manager
    retrieve_settings();
    set_border_width(20);

    // set parameters for orientation and spacing of boxes
    layout.set_orientation(Gtk::ORIENTATION_VERTICAL);
    format_box.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    bitrate_box.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    framerate_box.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    layout.set_spacing(6);
    format_box.set_spacing(6);
    bitrate_box.set_spacing(6);
    framerate_box.set_spacing(6);

    // add the layout and add the dropdown lists to the window
    add(layout);
    layout.show();
    layout.pack_start(format_box);
    format_box.show();
    layout.pack_start(bitrate_box);
    bitrate_box.show();
    layout.pack_start(framerate_box);
    framerate_box.show();
    format_box.pack_start(format_lbl);
    format_lbl.show();
    format_box.pack_start(format_dd);
    format_dd.show();
    bitrate_box.pack_start(bitrate_lbl);
    bitrate_lbl.show();
    bitrate_box.pack_start(bitrate_dd);
    bitrate_dd.show();
    framerate_box.pack_start(framerate_lbl);
    framerate_lbl.show();
    framerate_box.pack_start(framerate_dd);
    framerate_dd.show();

    // set text for labels
    format_lbl.set_text("File Format");
    bitrate_lbl.set_text("Bitrate");
    framerate_lbl.set_text("Framerate");

    // vectors useful in setting initial active based on the settigns file
    std::vector<std::string> format_vec{"mp4", "wmv"};
    std::vector<std::string> bitrate_vec{"2500", "4000", "6000"};
    std::vector<std::string> framerate_vec{"24", "30", "60"};

    // append options to dropdowns
    format_dd.append("mp4");
    format_dd.append("wmv");
    bitrate_dd.append("2500");
    bitrate_dd.append("4000");
    bitrate_dd.append("6000");
    framerate_dd.append("24");
    framerate_dd.append("30");
    framerate_dd.append("60");
    // get the index of the format setting
    for (int i = 0; i < format_vec.size(); ++i)
    {
        if (format_vec[i] == settings_list[0])
        {
            format_dd.set_active(i);
        }
    }
    // get the index of the bitrate setting
    for (int i = 0; i < bitrate_vec.size(); ++i)
    {
        if (bitrate_vec[i] == settings_list[1])
        {
            bitrate_dd.set_active(i);
        }
    } 
    // get the index of the framerate setting
    for (int i = 0; i < framerate_vec.size(); ++i)
    {
        if (framerate_vec[i] == settings_list[2])
        {
            framerate_dd.set_active(i);
        }
    }
    // create signal handlers for the dropdowns
    format_dd.signal_changed().connect(
        sigc::mem_fun(*this, &Settings_Window::format_changed));
    bitrate_dd.signal_changed().connect(
        sigc::mem_fun(*this, &Settings_Window::bitrate_changed));
    framerate_dd.signal_changed().connect(
        sigc::mem_fun(*this, &Settings_Window::framerate_changed));

    // add the save button to the window
    layout.pack_start(save_btn);
    save_btn.set_label("Apply");
    save_btn.show();
    // create signal handler for the save button
    save_btn.signal_clicked().connect(
        sigc::mem_fun(*this, &Settings_Window::apply_settings));
    
}
// destuctor
Settings_Window::~Settings_Window()
{
    observers.clear();
    delete settings_file_manager;
}
// get_settings
std::vector<std::string>& Settings_Window::get_settings()
{
    return settings_list;
}
// retrieve settings
void Settings_Window::retrieve_settings() 
{
    settings_list = settings_file_manager->load_settings();
}

// apply_settings
void Settings_Window::apply_settings()
{
    settings_file_manager->save_settings(settings_list);
    notify_observers(0);
}

// format_changed
void Settings_Window::format_changed()
{
    settings_list[0] = format_dd.get_active_text().raw();
}

// bitrate_changed
void Settings_Window::bitrate_changed()
{
    settings_list[1] = bitrate_dd.get_active_text().raw();
}

// framerate_changed
void Settings_Window::framerate_changed()
{
    settings_list[2] = framerate_dd.get_active_text().raw();
}

// attach
void Settings_Window::attach(Observer& o)
{
    observers.push_back(&o);
}

// detach
void Settings_Window::detach(Observer& o)
{
    int i = 0;
    while (observers[i] != &o) ++i;
    observers.erase(observers.begin() + i);
}

// notify_observers
void Settings_Window::notify_observers(int s)
{
    // settings window upon notifying observers should put the application state
    // back to a non-recording state.
    for (Observer* o : observers) o->update(s);
}

