#include <thread>
#include "Recorder.h"
#include "VideoStream.h"

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

void Recorder::start_recording(std::vector<std::string> settings)
{
    // create a new thread for the recorder to run on
    std::thread rec_thread(record, settings[1], settings[2], settings[0]);
    is_recording = true;
}

void Recorder::stop_recording()
{
    // interrupt the stream
    // after capture frame ends then the thread will end.
    media_stream->interrupt_stream();
}

void Recorder::record(std::string bitrate, std::string framerate, std::string format)
{
    media_stream->open_input(framerate);
    media_stream->create_outfile(bitrate, framerate, format);
    media_stream->capture_frame();
}

bool Recorder::get_recording()
{
    return is_recording;
}

void Recorder::attach(Observer& o)
{
    this->observers.push_back(&o);
} int i = 0;
    while (this->observers[i] != &o) ++i;
    this->observers.erase(this->observers.begin() + i);

void Recorder::detach(Observer& o)
{
    int i = 0;
    while (this->observers[i] != &o) ++i;
    this->observers.erase(this->observers.begin() + i);
}

void Recorder::notify_observers(int s)
{
    for (Observer* o : observers) o->update(s);
}
