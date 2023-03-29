#ifndef _APPLICATION_STATE_H_
#define _APPLICATION_STATE_H_

#include "Recorder.h"
#include "MainWindow.h"

class Application_State
{
public:
    virtual ~Application_State() = 0;
    virtual void handle(Recorder& recorder, Main_Window& main_window) = 0;
};

#endif