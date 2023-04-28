#ifndef _MEDIA_STREAM_H_
#define _MEDIA_STREAM_H_

#include <string>
// FFmpeg libraries are in c, define these with external linkage
#define __STDC_CONSTANT_MACROS
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavcodec/avfft.h>
#include <libavdevice/avdevice.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libavutil/opt.h>
#include <libavutil/common.h>
#include <libavutil/channel_layout.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
#include <libavutil/time.h>
#include <libavutil/pixdesc.h>
#include <libavutil/file.h>
#include <libswscale/swscale.h>
}


class Media_Stream
{
public:
    // destructor
    virtual ~Media_Stream() = 0;
    // open_input responsible for connecting ffmpeg with a media device
    virtual void open_input(std::string framerate) = 0;
    // capture_frame responsible for capturing frames from the media device and writing them to a file
    virtual void capture_frame(std::string framerate) = 0;
    // create_outfile responsible for creating an output file that can be written to
    virtual void create_outfile(std::string bitrate, std::string framerate, std::string format) = 0;
    // interrupt_stream responsible for setting an interrupt variable that will lead to ending a thread
    virtual void interrupt_stream() = 0;
};

#endif