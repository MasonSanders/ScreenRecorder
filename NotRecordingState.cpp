#include "NotRecordingState.h"
#include "RecorderBtnFactory.h"
#include <iostream>

// constructor
Not_Recording_State::Not_Recording_State()
: btn_factory(new Recorder_Btn_Factory())
{}

// destructor
Not_Recording_State::~Not_Recording_State()
{
    delete btn_factory;
}

// handle
void Not_Recording_State::handle(Recorder& recorder, Main_Window& main_window, Settings_Window& settings_window)
{
    // stop the recording if one is in progress
    if (recorder.get_recording())
    {
        recorder.stop_recording();
    }
    // show the main window and make it sensitive
    main_window.show();
    main_window.set_sensitive(true);

    // hide the settings window and set the main_window's buttons
    settings_window.hide();
    main_window.set_buttons(btn_factory->create_not_recording_btns());
}