#include "VideoStream.h"
#include <stdexcept>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <iostream>
#include <cstdlib>

// Constuctor
Video_Stream::Video_Stream() 
{
    av_register_all();
    avcodec_register_all();
    avdevice_register_all();
}

// Destructor
Video_Stream::~Video_Stream()
{
    avformat_close_input(&format_context);
    avformat_free_context(format_context); 
    avcodec_free_context(&out_codec_context);
    avcodec_free_context(&codec_context);
    avformat_free_context(out_format_context);
    av_frame_free(&frame);
}

// open_input responsible for establishing the connection between the ffmpeg api and the screen
void Video_Stream::open_input(std::string framerate)
{
    // allocate memory for the format context
    recording_options = NULL;
    if (format_context) format_context = NULL;
    format_context = avformat_alloc_context();
    int val;

    // use x11grab for linux, dshow for windows and
    // this will allow the program to compile differently depending on the platform
    // so that you don't have to go in and edit the input format manually.
    #ifdef __linux__
        in_format = av_find_input_format("x11grab");
        val = avformat_open_input(&format_context, getenv("DISPLAY"), in_format, NULL);
    #elif _WIN32
        in_format = av_find_input_format("gdigrab");
        av_dict_set(&recording_options, "draw_mouse", "1", 0);
        av_dict_set(&recording_options, "desktop", "1", 0);
        if (framerate == "24")
        {
            av_dict_set(&recording_options, "capture_rate", "24", 0);
        }
        else if (framerate == "30")
        {
            av_dict_set(&recording_options, "capture_rate", "30", 0);
        }
        else
        {
            av_dict_set(&recording_options, "capture_rate", "60", 0);
        }
    #else
        // avformat for MacOS not implemented
        in_format = av_find_input_format("avfoundation");
    #endif
        if (val != 0) throw std::runtime_error("cannot open input");

    // set framerate in dictionary for linux
    #ifdef __linux__
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
        if (val < 0) throw std::runtime_error("cannot change dictionary settigs");
    #endif
    val = av_dict_set(&recording_options, "preset", "medium", 0);
    if (val < 0) throw std::runtime_error("cannot change dictionary settings");

    // find the index in the format context of the stream with the correct codec type
    stream_idx = -1;
    for (int i = 0; i < format_context->nb_streams; ++i)
    {
        if (format_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            stream_idx = i;
            break;
        }
    }
    if (stream_idx == -1) throw std::runtime_error("cannot find video stream index");

    // set the codec_context and allocate the codec
    codec_context = format_context->streams[stream_idx]->codec;
    codec = avcodec_find_decoder(codec_context->codec_id);
    if (codec == NULL) throw std::runtime_error("cannot find codec");
    // open the codec
    val = avcodec_open2(codec_context, codec, NULL);
    if (val < 0) throw std::runtime_error("cannot open av codec");
}

void Video_Stream::capture_frame(std::string framerate)
{
    // adjust the time base (input from the monitor) to 60 fps
    input_stream->time_base.den = 3000 * input_stream->codec->time_base.den;

    interrupt = false;
    int flag;
    // used to tell whether or not a frame has finished being captured
    int frame_finished;
    int val = 0;
    
    // define an out_buffer and its size
    int out_buffer_size = av_image_get_buffer_size(out_codec_context->pix_fmt, out_codec_context->width, out_codec_context->height, 32);
    uint8_t* out_buffer;
    // allocate the packet
    packet = (AVPacket*)av_malloc(sizeof(AVPacket));
    av_init_packet(packet);

    // get context for scaling
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

        // Allocate a frame, out_frame, and allocate the out_buffer fill the frame with an image
        frame = av_frame_alloc();
        if (!frame) throw std::runtime_error("unable to allocate a frame");
        AVFrame* out_frame = av_frame_alloc();
        if (!out_frame) throw std::runtime_error("unable to allocate out frame");

        out_frame->format = out_codec_context->pix_fmt;
        out_frame->width = out_codec_context->width;
        out_frame->height = out_codec_context->height;
        out_buffer = (uint8_t*)av_malloc(out_buffer_size);
        if (out_buffer == NULL) throw std::runtime_error("cannot allocate output buffer");
        val = av_image_fill_arrays(out_frame->data, out_frame->linesize, out_buffer, AV_PIX_FMT_YUV420P, out_codec_context->width, out_codec_context->height, 1);
        if (val < 0) throw std::runtime_error("cannot fill image array");


        if (packet->stream_index == stream_idx)
        {
            // decode video from the input
            val = avcodec_decode_video2(codec_context, frame, &frame_finished, packet);
            if (val < 0) throw std::runtime_error("cannot decode video");
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
                // initialize the out_packet
                av_init_packet(&out_packet);
                out_packet.data = NULL;
                out_packet.size = 0;

                // encode video for the output
                avcodec_encode_video2(out_codec_context, &out_packet, out_frame, &got_packet); 
                if (got_packet)
                {
                    // convert timestamps so that the video appears in real time
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
                    out_packet.stream_index = stream_idx;
                    // write the frame to the output file
                    if (av_write_frame(out_format_context, &out_packet) != 0)
                        throw std::runtime_error("error writing video frame");
                }
                // get rid of the packets so that memory doesn't spike out of control during the loop
                av_packet_unref(packet);
                av_packet_unref(&out_packet);
            }
        }

        //free memory from frame out_frame, out_packet
        av_frame_free(&frame);
        av_frame_free(&out_frame);
        av_free(out_buffer);
        av_free(packet);
        // reallocate the packet so that it can be used again in the next loop
        packet = (AVPacket*)av_malloc(sizeof(AVPacket));
        av_init_packet(packet);

    }
    // after loop has exited return interrupt to its original state so that it may be used again.
    interrupt = false;

    // write trailer for file
    val = av_write_trailer(out_format_context);
    if (val < 0) throw std::runtime_error("cannot write to file");

    // free data at the end of recording
    avformat_close_input(&format_context);
    avformat_free_context(format_context);
    avcodec_free_context(&out_codec_context);
    av_free(packet);
}

// responsible for creating the output file which also controls bitrate, framerate and file format.
void Video_Stream::create_outfile(std::string bitrate, std::string framerate, std::string format)
{
    out_format_context = NULL;
    out_codec = NULL;
    int val = 0;
    // the name of the output file should be a timestamp for
    auto t = std::time(nullptr); 
    auto tt = *std::localtime(&t);
    std::stringstream ss;
    ss << std::put_time(&tt, "%d-%m-%Y %H-%M-%S");
    std::string outfile = ss.str();
    outfile += "." + format;
    // convert the outfile to a c string
    char* coutfile = const_cast<char*>(outfile.c_str());

    // allocate output format, video stream, and the output codec context
    avformat_alloc_output_context2(&out_format_context, NULL, NULL, coutfile);
    if (!out_format_context) throw std::runtime_error("cannot allocate output format context");
    out_format = av_guess_format(NULL, coutfile, NULL);
    if (!out_format) throw std::runtime_error("cannot get the output format");
    input_stream = avformat_new_stream(out_format_context, NULL);
    if (!input_stream) throw std::runtime_error("cannot create new stream");
    out_codec_context = avcodec_alloc_context3(out_codec);
    if (!out_codec_context) throw std::runtime_error("cannot allocate codec context");
    // ---------------------------------------------------------------------

    out_codec_context = input_stream->codec;
    // decide whether to use mp4 codec or wmv
    if (format == "mp4") out_codec_context->codec_id = AV_CODEC_ID_MPEG4;
    else out_codec_context->codec_id = AV_CODEC_ID_WMV2;

    out_codec_context->codec_type = AVMEDIA_TYPE_VIDEO;
    out_codec_context->pix_fmt = AV_PIX_FMT_YUV420P;

    // convert the bitrate to an integer to be used
    // this value is in bits/sec, convert to kb/s
    out_codec_context->bit_rate = std::stoi(bitrate) * 1000;
    // resolution of the outputted video
    out_codec_context->width = 1366;
    out_codec_context->height = 768;

    out_codec_context->gop_size = 50;
    out_codec_context->max_b_frames = 2;
    // time_base is the framerate which is num/den, 30 fps is 1/30
    out_codec_context->time_base.num = 1;
    out_codec_context->time_base.den = std::stoi(framerate);

    // choose the correct codec based on the video file type
    if (format == "mp4") out_codec = avcodec_find_encoder(AV_CODEC_ID_MPEG4);
    else out_codec = avcodec_find_encoder(AV_CODEC_ID_WMV2);


    if (!out_codec) throw std::runtime_error("Cannot find codec");

    if (out_format_context->oformat->flags & AVFMT_GLOBALHEADER)
        out_codec_context->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    
    // open the output codec
    val = avcodec_open2(out_codec_context, out_codec, NULL);
    if (val < 0) throw std::runtime_error("cannot open codec");

    // create the video file
    if (!(out_format_context->flags & AVFMT_NOFILE))
    {
        if (avio_open2(&out_format_context->pb, coutfile, AVIO_FLAG_WRITE, NULL, NULL) < 0)
        {
            throw std::runtime_error("cannot create file");
        }
    }

    if (!out_format_context->nb_streams) throw std::runtime_error("file doesn't have any streams");


    // write a required header for the file
    val = avformat_write_header(out_format_context, &recording_options);
    if (val < 0) throw std::runtime_error("cannot write header");
}

// interrupt stream
void Video_Stream::interrupt_stream()
{
    interrupt = true;
}