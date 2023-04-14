#ifndef _SCREEN_RECORDER_H_
#define _SCREEN_RECORDER_H_

#include <gtkmm.h>
#include <vector>
#include <string>
#include "MainWindow.h"
#include "SettingsWindow.h"
#include "Observer.h"
#include "Recorder.h"
#include "ApplicationState.h"

class Screen_Recorder : public Gtk::Application, public Observer 
{
public:
    Screen_Recorder();

    ~Screen_Recorder();

    int run() override;

    void open_settings_win();

    void update(int s) override;

    Application_State* get_state();

private:
    Main_Window* main_window;

    Settings_Window * settings_window;

    Recorder* recorder;

    std::vector<Application_State*> states;

    int cur_state;
};

#endif