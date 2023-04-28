#ifndef _NOT_RECORDING_STATE_H_
#define _NOT_RECORDING_STATE_H_

#include "ApplicationState.h"
#include "AbstractRecorderBtnFactory.h"

class Not_Recording_State : public Application_State
{
public:
    // constructor
    Not_Recording_State();
    // destructor
    ~Not_Recording_State();

    // handle responsible for setting the state
    void handle(Recorder& recorder, Main_Window& main_window, Settings_Window& settings_window) override;

private:
    Abstract_Recorder_Btn_Factory* btn_factory;
};
#endif