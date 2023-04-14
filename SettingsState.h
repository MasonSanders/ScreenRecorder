#ifndef _SETTINGS_STATE_H_
#define _SETTINGS_STATE_H_

#include "ApplicationState.h"

class Settings_State : public Application_State
{
public:
    Settings_State();
    ~Settings_State();
    
    void handle(Recorder& recorder, Main_Window& main_window, Settings_Window& settings_window) override;

};
#endif