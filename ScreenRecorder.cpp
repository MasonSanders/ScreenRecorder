#include "ScreenRecorder.h"
#include "NotRecordingState.h"
#include "RecordingState.h"
#include "SettingsState.h"
#include <exception>
#include <iostream>

// constructor
Screen_Recorder::Screen_Recorder()
: Gtk::Application(),
  recorder(new Recorder()),
  states{new Not_Recording_State(), new Recording_State(), new Settings_State()}
{

}

// destructor
Screen_Recorder::~Screen_Recorder()
{
    delete main_window;
    delete settings_window;
    delete recorder;
    for (Application_State* state : states)
    {
        delete state;
    }
    states.clear();
}

// on_activate called from gtk::application::run, when signal_activate is emitted
void Screen_Recorder::on_activate() 
{   
    // initialize main_window and settings window here instead of the constructor
    // since if gtk objects are instantiated before signal_activcate is emitted, there would be an error.
    main_window = new Main_Window();
    settings_window = new Settings_Window();
    add_window(*settings_window);
    add_window(*main_window);

    // attach this as an observer to the subjects
    recorder->attach(*this);
    main_window->attach(*this);
    settings_window->attach(*this);

    // connect main_window's hide signal to on_main_close so that the application can exit properly
    main_window->signal_hide().connect(
        sigc::mem_fun(*this, &Screen_Recorder::on_main_close));
    
    // start in a not recording state and handle
    states[0]->handle(*recorder, *main_window, *settings_window);
}

// update
void Screen_Recorder::update(int s)
{
    // update gets passed an int s which represents the state.
    states[s]->handle(*recorder, *main_window, *settings_window);
}

// on_main_close
void Screen_Recorder::on_main_close()
{
    // adding the settings window right before quitting ensures that the application exits without a segmentation fault
    add_window(*settings_window);
    quit();

}
