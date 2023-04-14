#ifndef _MEDIA_STREAM_H_
#define _MEDIA_STREAM_H_

#include <string>
// FFmpeg libraries are in c, define these with external linkage
#define __STDC_CONSTANT_MACROS
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavcodec/avfft.h"
#include "libavdevice/avdevice.h"
#include "libavfilter/avfilter.h"
#include "libavfilter/avfiltergraph.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
#include "libavformat/avformat.h"
#include "libavformat/avio.h"
#include "libavutil/opt.h"
#include "libavutil/common.h"
#include "libavutil/channel_layout.h"
#include "libavutil/imgutils.h"
#include "libavutil/mathematics.h"
#include "libavutil/samplefmt.h"
#include "libavutil/time.h"
#include "libavutil/pixdesc.h"
#include "libavutil/file.h"
#include "libswscale/swscale.h"
}

class Media_Stream
{
public:
    virtual ~Media_Stream() = 0;
    virtual void open_input(std::string framerate) = 0;
    virtual void capture_frame() = 0;
    virtual void create_outfile(std::string bitrate, std::string framerate, std::string format) = 0;
    virtual void interrupt_stream() = 0;
};

#endif