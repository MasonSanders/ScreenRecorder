#include "SettingsState.h"

Settings_State::Settings_State() {}
Settings_State::~Settings_State() {}

void Settings_State::handle(Recorder& recorder, Main_Window& main_window, Settings_Window& settings_window)
{
    settings_window.show();
    main_window.hide();
}