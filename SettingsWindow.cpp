#include "SettingsWindow.h"

Settings_Window::Settings_Window()
: Gtk::Window(),
  settings_list{"mp4", "4000", "30"},
  settings_file_manager(new Settings_File_Manager())
{
    set_default_size(400, 400);
    set_title("Edit Settings");
    retrieve_settings();

    set_border_width(20);
    layout.set_orientation(Gtk::ORIENTATION_VERTICAL);
    Gtk::Box format_box;
    format_box.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    Gtk::Box bitrate_box;
    bitrate_box.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    Gtk::Box framerate_box;
    framerate_box.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    layout.set_spacing(6);
    // add the layout and add the dropdown lists to the window
    add(layout);
    layout.add(format_box);
    layout.add(bitrate_box);
    layout.add(framerate_box);
    format_box.add(format_lbl);
    format_box.add(format_dd);
    bitrate_box.add(bitrate_lbl);
    bitrate_box.add(bitrate_dd);
    framerate_box.add(framerate_lbl);
    framerate_box.add(framerate_dd);

    format_lbl.set_text("File Format");
    bitrate_lbl.set_text("Bitrate");
    framerate_lbl.set_text("Framerate");
    std::vector<std::string> format_vec{"mp4", "wmv"};
    std::vector<std::string> bitrate_vec{"2500", "4000", "6000"};
    std::vector<std::string> framerate_vec{"24", "30", "60"};

    // create the lists for the dropdowns and add them to the dropdowns
    format_list = Gtk::StringList::create(format_vec);
    bitrate_list = Gtk::StringList::create(bitrate_vec);
    framerate_list = Gtk::StringList::create(framerate_vec);
    format_dd.set_model(format_list);
    // get the index of the format setting
    for (int i = 0; i < format_vec.size(); ++i)
    {
        if (format_vec[i] == settings_list[0])
        {
            format_dd.set_selected(i);
        }
    }
    bitrate_dd.set_model(bitrate_list);
    // get the index of the bitrate setting
    for (int i = 0; i < bitrate_vec.size(); ++i)
    {
        if (bitrate_vec[i] == settings_list[1])
        {
            bitrate_dd.set_selected(i);
        }
    } 
    framerate_dd.set_model(framerate_list);
    // get the index of the framerate setting
    for (int i = 0; i < framerate_vec.size(); ++i)
    {
        if (bitate_vec[i] == settings_list[2])
        {
            framerate_dd.set_selected(i);
        }
    }
    // create signal handlers for the dropdowns
    format_dd.property_selected().signal_changed().connect(
        sigc::mem_fun(*this, &Settings_Window::format_changed));
    bitrate_dd.property_selected().signal_changed().connect(
        sigc::mem_fun(*this, &Settings_Window::bitrate_changed));
    framerate_dd.property_selected().signal_changed().connect(
        sigc::mem_fun(*this, &Settings_Window::framerate_changed));

    // add the save button to the window
    layout.add(save_btn);
    save_btn.set_label("Apply");
    // create signal handler for the save button
    save_btn.signal_clicked().connect(
        sigc::mem_fun(*this, &Settings_Window::apply_settings));
}

Settings_Window::~Settings_Window()
{
    observers.clear();
    delete settings_file_manager;
}

std::vector<std::string>& Settings_Window::get_settings()
{
    return settings_list;
}

void Settings_Window::retrieve_settings() 
{
    settings_list = settings_file_manager->load_settings();
}

void Settings_Window::apply_settings()
{
    settings_file_manager->save_settings(settings_list);
    notify_observers(0);
}

void Settings_Window::format_changed()
{
    settings_list[0] = format_list->get_string(format_dd.get_selected());
}

void Settings_Window::bitrate_changed()
{
    settings_list[1] = bitrate_list->get_string(bitrate_dd.get_selected());
}

void Settings_Window::framerate_changed()
{
    settings_list[2] = framerate_list->get_string(framerate_dd.get_selected());
}

void Settings_Window::attach(Observer& o)
{
    observers.push_back(&o);
}

void Settings_Window::detach(Observer& o)
{
    int i = 0;
    while (observers[i] != &o) ++i;
    observers.erase(observers.begin() + i);
}

void Settings_Window::notify_observers(int s)
{
    // settings window upon notifying observers should put the application state
    // back to a non-recording state.
    for (Observer* o : observers) o->update(s);
}

