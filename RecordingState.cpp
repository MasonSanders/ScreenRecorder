#include "RecordingState.h"

#include "RecorderBtnFactory.h"

// constructor
Recording_State::Recording_State()
: btn_factory(new Recorder_Btn_Factory())
{

}

// destructor
Recording_State::~Recording_State()
{
    delete btn_factory;
}

// handle
void Recording_State::handle(Recorder& recorder, Main_Window& main_window, Settings_Window& settings_window)
{
    // set the recording buttons and start the recording
    main_window.set_buttons(btn_factory->create_recording_btns());
    recorder.start_recording(settings_window.get_settings());
}