#include "ScreenRecorder.h"
#include <exception>

Screen_Recorder::Screen_Recorder()
: main_window(new Main_Window()),
  settings_window(new Settings_Window()),
  recorder(new Recorder()),
  states{new Not_Recording_State(), new Recording_State(), new Settings_State()},
  cur_state(0)
{}

Screen_Recorder::~Screen_Recorder()
{
    delete main_window;
    delete settings_window;
    delete recorder;
    for (Application_State* state : states)
    {
        delete s;
    }
    states.clear();
}

// responsible for returning the exit state of the application
int Screen_Recorder::run() 
{   
    try 
    {
        main_window->show();
    }
    catch (std::exception& e)
    {
        return 1;
    }
    return 0;
}

void Screen_Recorder::update(int s)
{
    // cannot just switch between states
    // update gets passed an int s which represents the state.
    states[s]->handle(*recorder, *main_window, *settings_window);
}

void Screen_Recorder::open_settings_win()
{
    this->settings_window->show();
}

Application_State* Screen_Recorder::get_state() 
{
    return this->states[this->cur_state];
}
