#include "VideoStream.h"
#include <exception>
#include <iomanip>
#include <ctime>
#include <sstream>

Video_Stream::Video_Stream() {}

Video_Stream::~Video_Stream()
{
    avformat_close_input(&format_context);
    avformat_free_context(format_context); 
}

// responsible for establishing the connection between the ffmpeg api and the screen
void Video_Stream::open_input(std::string framerate)
{
    recording_options = NULL;
    if (format_context) format_context = NULL;
    format_context = avformat_alloc_context();

    // use x11grab for linux, dshow for windows and
    // this will allow the program to compile differently depending on the platform
    // so that you don't have to go in and edit the input format manually.
    #ifdef __linux__
        in_format = av_find_input_format("x11grab");
    #elif _WIN32
        in_format = av_find_input_format("dshow");
    #else
        in_format = av_find_input_format("avfoundation");
    #endif

    int val = avformat_open_input(&format_context, ":0.0+10,250", in_format, NULL);
    if (val != 0) throw std::exception("cannot open input");
    // since I saved the framerate as a std::string, it's easier use if statements
    // rather than converting
    if (framerate == "24")
    {
        val = av_dict_set(&recording_options, "framerate", "24", 0);
    }
    else if (framerate == "30")
    {
        val = av_dict_set(&recording_options, "framerate", "30", 0);
    }
    else
    {
        val = av_dict_set(&recording_options, "framerate", "60", 0);
    }
    if (val < 0) throw std::exception("cannot change dictionary settigs");
    val = av_dict_set(&options, "preset", "medium", 0);
    if (val < 0) throw std::exception("cannot change dictionary settings");
    stream_idx = -1;
    for (int i = 0; i < format_context->nb_streams; ++i)
    {
        if (format_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            stream_idx = i;
            break;
        }
    }
    if (stream_idx == -1) throw std::exception("cannot find video stream index");
    codec_context = format_context->streams[stream_idx]->codec;
    codec = avcodec_find_decoder(codec_context->codec_id);
    if (codec == NULL) throw std::exception("cannot find codec");
    val = avcodec_open2(codec_context, codec, NULL);
    if (val < 0) throw std::exception("cannot open av codec");
}

void Video_Stream::capture_frame()
{
    int flag;
    // used to tell whether or not a frame has finished being captured
    int frame_finished;

    int frame_idx = 0;
    int val = 0;
    packet = (AVPacket*)av_malloc(sizeof(AVPacket));
    av_init_packet(packet);
    frame = av_frame_alloc();
    if (!frame) throw std::exception("unable to allocate a frame");
    AVFrame* out_frame = av_frame_alloc();
    if (!out_frame) throw std::exception("unable to allocate out frame");
    int out_buffer_size = av_image_get_buffer_size(out_codec_context->pix_fmt, out_codec_context->width, out_codec_context->height, 32);
    uint8_t* out_buffer = (uint8_t*)av_malloc(out_buffer_size);
    if (out_buffer == NULL) throw std::exception("cannot allocate output buffer");
    val = av_image_fill_arrays(out_frame->data, out_frame->linesize, out_buffer, AV_PIX_FMT_YUV420P, out_codec_context, out_codec_context->width, out_codec_context->height, 1);
    if (val < 0) throw std::exception("cannot fill image array");
    SwsContext* sws;
    sws = sws_getContext(
        codec_context->width, 
        codec_context->height, 
        codec_context->pix_fmt, 
        out_codec_context->width, 
        out_codec_context->height, 
        out_codec_context->pix_fmt, 
        SWS_BICUBIC, 
        NULL, 
        NULL, 
        NULL);

    // run the loop of recording video
    AVPacket out_packet;
    int got_packet;
    while(av_read_frame(format_context, packet) >= 0)
    {
        // break and quit recording if interrupt is true
        if (interrupt) break;
        if (packet->stram_index == stream_idx)
        {
            val = avcodec_decode_video2(codec_context, frame, &frame_finshed, packet);
            if (val < 0) throw std::exception("cannot decode video");
            if (frame_finished)
            {
                sws_scale(
                    sws, 
                    frame->data, 
                    frame->linesize, 
                    0, 
                    codec_context->height, 
                    out_frame->data, 
                    out_frame->linesize);

                av_init_paket(&out_packet);
                out_packet.data = NULL;
                out_packet.size = 0;

                avcodec_encode_video2(out_codec_context, &out_packet, out_frame, &got_packet); 
                if (got_packet)
                {
                    if (out_packet.pts != AV_NOPTS_VALUE)
                    {
                        out_packet.pts = av_rescale_q(
                            out_packet.pts, 
                            input_stream->codec->time_base, 
                            input_stream->time_base);
                    }
                    if (out_packet.dts != AV_NOPTS_VALUE)
                    {
                        out_packet.dts = av_rescale_q(
                            out_packet.dts, 
                            input_stream->codec->time_base, 
                            input_stream->time_base);
                    }
                    av_packet_unref(&out_packet);
                }
                av_packet_unref(&out_packet);
            }
        }
    }
    // after loop has exited return interrupt to its original state so that it may be used again.
    interrupt = false

    val = av_write_trailer(out_format_context);
    if (val < 0) throw std::exception("cannot write to file");
    av_free(out_buffer);
}

// responsible for creating the output file which also controls bitrate, framerate and file format.
void Video_Stream::create_outfile(std::string bitrate, std::string framerate, std::string format);
{
    out_format_context = NULL;
    int val = 0;
    // the name of the output file should be a timestamp for
    auto t = std::time(nullptr); 
    auto tt = *std::localtime(&t);
    std::stringstream ss;
    ss << std::put_time(&tt, "%d-%m-%Y %H-%M-%S");
    std::string outfile = ss.str();
    std::string outfile += "." + format;
    // convert the outfile to a c string
    char* coutfile = const_cast<char*>(outfile.c_str());

    avformat_alloc_output_context2(&out_format_context, NULL, NULL, coutfile);
    if (!out_format_context) throw std::excpetion("cannot allocate output format context");
    out_format = av_guess_format(NULL, coutfile, NULL);
    if (!out_format) throw std::exception("cannot get the output format");
    input_stream = avformat_new_stream(out_format_context, NULL);
    if (!input_stream) throw std::exception("cannot create new stream");
    out_codec_context = avcodec_alloc_context3(out_codec);
    if (!out_codec_context) throw std::exception("cannot allocate codec context");

    out_codec_context = input_stream->codec;
    // decide whether to use mp4 codec or wmv
    if (format = "mp4") out_codec_context->codec_id = AV_CODEC_ID_MPEG4;
    else out_codec_context->codec_id = AV_CODEC_ID_WMV2;

    out_codec_context->codec_type = AVMEDIA_TYPE_VIDEO;
    out_codec_context->pix_fmt = AV_PIX_FMT_YUV420P;

    // convert the bitrate to an integer to be used
    out_codec_context->bit_rate = std::stoi(bitrate);

    // resolution of the outputted video
    out_codec_context->width = 1920;
    out_codec_context->height = 1080;

    out_codec_context->gop_size = 3;
    out_codec_context->max_b_frames = 2;
    // time_base is the framerate which is num/den, 30 fps is 1/30
    out_codec_context->time_base.num = 1;
    out_codec_context->time_base.den = std::stoi(framerate);

    if (format == "mp4") out_codec = avcodec_find_encoder(AV_CODEC_ID_MPEG4);
    else out_codec = avcodec_find_encoder(AV_CODEC_ID_WMV2);

    if (!out_codec) throw std::exception("Cannot find codec");

    if (out_format_context->oformat->flags & AVFMT_GLOBALHEADER)
        out_codec_context |= AV_CODEC_FLAG_GLOBAL_HEADER;
    
    val = avformat_write_header(out_format_context, &recording_options);
    if (val < 0) throw std::exception("cannot write header");
}

void Video_Stream::interrupt_stream()
{
    interrupt = true;
}