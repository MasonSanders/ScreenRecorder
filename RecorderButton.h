#ifndef _RECORDER_BUTTON_H_
#define _RECORDER_BUTTON_H_

#include <gtkmm.h>

class Recorder_Button : public Gtk::Button
{
public:
    Recorder_Button();
    virtual ~Recorder_Button() = 0;
}

#endif