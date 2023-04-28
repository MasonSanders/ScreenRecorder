#include "RecorderBtnFactory.h"
#include "StartRecordButton.h"
#include "StopRecordButton.h"
#include "SettingsButton.h"

Recorder_Btn_Factory::Recorder_Btn_Factory()
{
    Start_Record_Button start_recording_btn;
    Settings_Button settings_btn;

    not_recording_btns.push_back(&start_recording_btn);
    not_recording_btns.push_back(&settings_btn);

    Stop_Record_Button stop_recording_btn;
    recording_btns.push_back(&stop_recording_btn);
}

Recorder_Btn_Factory::~Recorder_Btn_Factory() 
{
    for (Gtk::Button* btn : recording_btns)
    {
        delete btn;
    }
    for (Gtk::Button* btn : not_recording_btns)
    {
        delete btn;
    }
}

std::vector<Gtk::Button*>& Recorder_Btn_Factory::create_recording_btns()
{
    return recording_btns;
}

std::vector<Gtk::Button*>& Recorder_Btn_Factory::create_not_recording_btns()
{
    return not_recording_btns;
}