#ifndef _APPLICATION_STATE_H_
#define _APPLICATION_STATE_H_

#include "Recorder.h"
#include "MainWindow.h"
#include "SettingsWindow.h"


class Application_State
{
public:
    // destructor
    virtual ~Application_State() = 0;
    // handle
    virtual void handle(Recorder& recorder, Main_Window& main_window, Settings_Window& settings_window) = 0;
};

#endif