#ifndef _RECORDER_BTN_FACTORY_H_
#define _RECORDER_BTN_FACTORY_H_

#include "AbstractRecorderBtnFactory.h"

class Recorder_Btn_Factory : public Abstract_Recorder_Btn_Factory
{
public:
    Recorder_Btn_Factory();
    ~Recorder_Btn_Factory();
    std::vector<Gtk::Button*>& create_recording_btns() override;
    std::vector<Gtk::Button*>& create_not_recording_btns() override;
private:
    std::vector<Gtk::Button*> recording_btns;
    std::vector<Gtk::Button*> not_recording_btns;
};

#endif