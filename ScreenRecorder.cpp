#include "ScreenRecorder.h"

Screen_Recorder::Screen_Recorder()
: main_window(new Main_Window()),
  settings_window(new Settings_Window()),
  recorder(new Recorder()),
  states{new Not_Recording_State(), new Recording_State()},
  cur_state(0)
{}

Screen_Recorder::~Screen_Recorder()
{
    delete this->main_window;
    delete this->settings_window;
    delete this->recorder;
    for (Application_State* state : this->states)
    {
        delete s;
    }
    this->states.clear();
}

// responsible for returning the exit state of the application
int Screen_Recorder::run() 
{   
    try 
    {
        this->main_window->show();
    }
    catch ()
    {
        return 1;
    }
    return 0;
}

void Screen_Recorder::update()
{
    this->cur_state = (this->cur_state + 1) % 2;
    this->states[this->cur_state]->handle(*(this->recorder), *(this->main_window));
}

void Screen_Recorder::open_settings_win()
{

}

Application_State* Screen_Recorder::get_state() 
{
    return this->states[this->cur_state];
}
