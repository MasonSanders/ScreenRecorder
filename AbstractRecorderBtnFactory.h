#ifndef _ABSTRACT_RECORDER_BTN_FACTORY_H_
#define _ABSTRACT_RECORDER_BTN_FACTORY_H_

#include <vector>
#include <gtkmm.h>

class Abstract_Recorder_Btn_Factory
{
public:
    // destructor
    virtual ~Abstract_Recorder_Btn_Factory() = 0;
    // create recording buttons
    virtual std::vector<Gtk::Button*>& create_recording_btns() = 0;
    // create not recording buttons
    virtual std::vector<Gtk::Button*>& create_not_recording_btns() = 0;
};

#endif