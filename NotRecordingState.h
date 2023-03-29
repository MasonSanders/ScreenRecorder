#ifndef _NOT_RECORDING_STATE_H_
#define _NOT_RECORDING_STATE_H_

#include "ApplicationState.h"
#include "AbstractRecorderBtnFactory.h"

class Not_Recording_State : public Application_State
{
public:
    Not_Recording_State();
    ~Not_Recording_State();

    void handle(Recorder& recorder, Main_Window& main_window) override;

private:
    Abstract_Recorder_Btn_Factory* btn_factory;
};
#endif