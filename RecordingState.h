#ifndef _RECORDING_STATE_H_
#define _RECORDING_STATE_H_

#include "ApplicationState.cpp"
#include "AbstractRecorderBtnFactory.h"

class Recording_State : public Application_State
{
public:
    Recording_State();
    ~Recording_State();

    void handle(Recorder& recorder, Main_Window& main_window) override;

private:
    Abstract_Recorder_Btn_Factory* btn_factory;
};

#endif