#include "NotRecordingState.h"
#include "RecorderBtnFactory.h"

Not_Recording_State::Not_Recording_State()
: btn_factory(new Recorder_Btn_Factory())
{}

Not_Recording_State::Not_Recording_State()
{
    delete btn_factory;
}

void Not_Recording_State::handle(Recorder& recorder, Main_Window& main_window, Settings_Window& settings_window)
{
    if (recorder.get_recording())
    {
        recorder.stop_recording();
    }
    settings_window.hide();
    main_window.show();
    main_window.set_buttons(btn_factory.create_not_recording_btns());
}