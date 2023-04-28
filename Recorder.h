#ifndef _RECORDER_H_
#define _RECORDER_H_

#include <vector>
#include <string>
#include <thread>
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

    // start_recording responsible for starting a thread to record on
    void start_recording(std::vector<std::string> settings);

    // stop_recording responsible for interrupting the recording thread
    void stop_recording();

    // record, the method called by a recording thread and handles the process of recording
    void record(std::string bitrate, std::string framerate, std::string format);

    // get_recording gets whether or not a recording is in progress
    bool get_recording();

    // attach
    void attach(Observer& o) override;

    // detach
    void detach(Observer& o) override;

    // notfy_observers
    void notify_observers(int s) override;
private:
    // observers vector
    std::vector<Observer*> observers;
    // media_stream
    Media_Stream* media_stream;
    bool is_recording;

    // thread to record on is a member variable so that it can be joined from a separate method that it's started in
    std::thread rec_thread;
};

#endif