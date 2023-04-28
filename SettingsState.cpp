#include "SettingsState.h"

// Constructor
Settings_State::Settings_State() {}

// Destructor
Settings_State::~Settings_State() {}

// handle
void Settings_State::handle(Recorder& recorder, Main_Window& main_window, Settings_Window& settings_window)
{
    // show the settings window and set the main window to be insensitive
    settings_window.show();
    main_window.set_sensitive(false);
}