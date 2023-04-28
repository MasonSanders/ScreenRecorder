#ifndef _RECORDING_STATE_H_
#define _RECORDING_STATE_H_

#include "ApplicationState.h"
#include "AbstractRecorderBtnFactory.h"

class Recording_State : public Application_State
{
public:
    // constructor
    Recording_State();
    // destructor
    ~Recording_State();
    // handle responsible for managing the state for recording
    void handle(Recorder& recorder, Main_Window& main_window, Settings_Window& settings_window) override;

private:
    Abstract_Recorder_Btn_Factory* btn_factory;
};

#endif