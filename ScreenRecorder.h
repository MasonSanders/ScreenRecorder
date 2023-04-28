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
    // Constructor
    Screen_Recorder();

    // Destuctor
    ~Screen_Recorder();

    // on_activate gets called from gtk::Application::run 
    void on_activate() override;

    // update
    void update(int s) override;

protected:
    // on_main_close reponsible for closing the application when the main_window is hidden
    void on_main_close();

private:
    // main window of the application
    Main_Window* main_window;

    // settings window of the application
    Settings_Window * settings_window;

    // the recorder model
    Recorder* recorder;

    // states vector responsible for holding states
    std::vector<Application_State*> states;
};

#endif