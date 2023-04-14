#ifndef _RECORDER_H_
#define _RECORDER_H_

#include <vector>
#include <string>
#include "Subject.h"
#include "Observer.h"
#include "MediaStream.h"

class Recorder : public Subject
{
public:
    // constructor
    Recorder();

    // destructor
    ~Recorder();

    void start_recording();

    void stop_recording();

    void record();

    bool get_recording();

    // attach
    void attach(Observer& o) override;

    // detach
    void detach(Observer& o) override;

    void notify_observers(int s) override;
private:
    std::vector<Observer*> observers;
    Media_Stream* media_stream;
    bool is_recording;
};

#endif