#include <thread>
#include "Recorder.h"
#include "VideoStream.h"
#include <iostream>

// Constructor
Recorder::Recorder() 
: observers(),
  media_stream(new Video_Stream()),
  is_recording(false)
{
    // register ffmpeg functions
    av_register_all();
    avcodec_register_all();
    avdevice_register_all();
}

// Destructor
Recorder::~Recorder() 
{
    delete media_stream;
    observers.clear();
}

// start recording
void Recorder::start_recording(std::vector<std::string> settings)
{
    // create a new thread for the recorder to run on, set is recording to true
    rec_thread = std::thread{&Recorder::record, this, settings[1], settings[2], settings[0]};
    is_recording = true;
}

// stop recording
void Recorder::stop_recording()
{
    // interrupt the stream
    // after capture frame ends then the thread will end.
    media_stream->interrupt_stream();
    is_recording = false;
    rec_thread.join();
}

// record
void Recorder::record(std::string bitrate, std::string framerate, std::string format)
{
    // open the input, create output file, and capture frames
    media_stream->open_input(framerate);
    media_stream->create_outfile(bitrate, framerate, format);
    media_stream->capture_frame(framerate);
}

bool Recorder::get_recording()
{
    return is_recording;
}

// attach
void Recorder::attach(Observer& o)
{
    observers.push_back(&o);
} 

// detach
void Recorder::detach(Observer& o)
{
    int i = 0;
    while (observers[i] != &o) ++i;
    observers.erase(observers.begin() + i);
}

// notify_observers
void Recorder::notify_observers(int s)
{
    for (Observer* o : observers) o->update(s);
}
