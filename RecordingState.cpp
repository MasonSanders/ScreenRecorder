#include "RecordingState.h"

Recording_State::Recording_State
{

}

Recording_State::~Recording_State
{

}

void Recording_State::handle(Recorder& recorder, Main_Window& main_window, Settings_Window& settings_window)
{
    main_window.set_buttons(btn_factory.create_recording_btns());
    recorder.start_recording();
}