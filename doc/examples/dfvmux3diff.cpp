/*
 * Copyright (c) 2010 Nicolas George
 * Copyright (c) 2011 Stefano Sabatini
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * @file
 * API example for decoding and filtering
 * @example decode_filter_video.c
 */
#define _XOPEN_SOURCE 600 /* for usleep */
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h> 
extern "C" {
  #include <libavcodec/avcodec.h>
  #include <libavformat/avformat.h>
  #include <libavfilter/buffersink.h>
  #include <libavfilter/buffersrc.h>
  #include <libavutil/opt.h>
}
//dfvmux.c ArithAI Yang 2024.12.16

#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)

// RGB -> YUV
#define RGB2Y(R, G, B) CLIP(( (  66 * (R) + 129 * (G) +  25 * (B) + 128) >> 8) +  16)
#define RGB2U(R, G, B) CLIP(( ( -38 * (R) -  74 * (G) + 112 * (B) + 128) >> 8) + 128)
#define RGB2V(R, G, B) CLIP(( ( 112 * (R) -  94 * (G) -  18 * (B) + 128) >> 8) + 128)

// YUV -> RGB
#define C(Y) ( (Y) - 16  )
#define D(U) ( (U) - 128 )
#define E(V) ( (V) - 128 )

#define YUV2R(Y, U, V) CLIP(( 298 * C(Y)              + 409 * E(V) + 128) >> 8)
#define YUV2G(Y, U, V) CLIP(( 298 * C(Y) - 100 * D(U) - 208 * E(V) + 128) >> 8)
#define YUV2B(Y, U, V) CLIP(( 298 * C(Y) + 516 * D(U)              + 128) >> 8)

// RGB -> YCbCr
#define CRGB2Y(R, G, B) CLIP((19595 * R + 38470 * G + 7471 * B ) >> 16)
#define CRGB2Cb(R, G, B) CLIP((36962 * (B - CLIP((19595 * R + 38470 * G + 7471 * B ) >> 16) ) >> 16) + 128)
#define CRGB2Cr(R, G, B) CLIP((46727 * (R - CLIP((19595 * R + 38470 * G + 7471 * B ) >> 16) ) >> 16) + 128)

// YCbCr -> RGB
#define CYCbCr2R(Y, Cb, Cr) CLIP( Y + ( 91881 * Cr >> 16 ) - 179 )
#define CYCbCr2G(Y, Cb, Cr) CLIP( Y - (( 22544 * Cb + 46793 * Cr ) >> 16) + 135)
#define CYCbCr2B(Y, Cb, Cr) CLIP( Y + (116129 * Cb >> 16 ) - 226 )

#define WHITEY 255
#define WHITEU 128
#define WHITEV 128
#define BLACKY 0
#define BLACKU 128
#define BLACKV 128

//const char *filter_descr = "scale=80:80,transpose=cclock";
//const char *filter_descr = "scale=32:108,transpose=cclock";
//const char *filter_descr = "scale=32:112,transpose=cclock";
//const char *filter_descr = "scale=1280:720";
//const char *filter_descr = "scale=32:32";
//const char *filter_descr = "scale=iw:ih";
#define GLOBAL_WIDTH  640*6
#define GLOBAL_HEIGHT 360*6
//#define GLOBAL_WIDTH  360*3
//#define GLOBAL_HEIGHT 640*3

typedef struct STPOINT {
  unsigned short x;  //0..3840-1
  unsigned short y;  //0..2160-1
  unsigned short nr; //0..65535
  int w;
} ST_POINT;
typedef struct STPONTLIST {
  ST_POINT pt;
  struct STPONTLIST *next;
} ST_POINT_LIST;
ST_POINT_LIST *ptHead=NULL,*ptTail=NULL;
void ptSet(unsigned short x,unsigned short y,int weight);
void ptFree();
void ptListAll(); 
ST_POINT_LIST *ptHeadG=NULL,*ptTailG=NULL;
void ptSetG(unsigned short x,unsigned short y,int weight);
void ptFreeG();
void ptListAllG(); 
#define maxFirst 32
ST_POINT_LIST *ptFirst;
void ptGetFirst();
ST_POINT_LIST *ptFirstG;
void ptGetFirstG();
char filter_descr[sizeof("scale=3840:2160,transpose=clock")];
/* other way:
   scale=78:24 [scl]; [scl] transpose=cclock // assumes "[in]" and "[out]" to be input output pads respectively
 */
static AVFormatContext *fmt_ctx;
static AVCodecContext *dec_ctx;
AVFilterContext *buffersink_ctx;
AVFilterContext *buffersrc_ctx;
AVFilterGraph *filter_graph;
static int video_stream_index = -1;
//static int64_t last_pts = AV_NOPTS_VALUE;
//mux begin/////////////////////////////////////////////////////////////////////
#include <string.h>
#include <math.h>
extern "C" {
  #include <libavutil/avassert.h> 
  #include <libavutil/channel_layout.h>
  #include <libavutil/opt.h>
  #include <libavutil/mathematics.h>
  #include <libavutil/timestamp.h>
  #include <libavcodec/avcodec.h>
  #include <libavformat/avformat.h>
  #include <libswscale/swscale.h>
  #include <libswresample/swresample.h>
}

#define STREAM_DURATION   10.0
#define STREAM_FRAME_RATE 25 /* 25 images/s */
#define STREAM_PIX_FMT    AV_PIX_FMT_YUV420P /* default pix_fmt */
#define SCALE_FLAGS SWS_BICUBIC

AVFrame *filt_frame;
int frameWidth,frameHeight;
unsigned char *Ybeforediff=NULL;
unsigned char *Ubeforediff=NULL;
unsigned char *Vbeforediff=NULL;
unsigned char *Ybefore=NULL;
unsigned char *Ubefore=NULL;
unsigned char *Vbefore=NULL;
unsigned char *Ydiffnow=NULL;
unsigned char *Udiffnow=NULL;
unsigned char *Vdiffnow=NULL;
unsigned char *Ynow=NULL;
unsigned char *Unow=NULL;
unsigned char *Vnow=NULL;
unsigned char *Yref=NULL;
unsigned char *Uref=NULL;
unsigned char *Vref=NULL;
int Ylinesize,Ulinesize,Vlinesize;
unsigned char *Rbefore=NULL;
unsigned char *Gbefore=NULL;
unsigned char *Bbefore=NULL;
unsigned char *Rnow=NULL;
unsigned char *Gnow=NULL;
unsigned char *Bnow=NULL;

// a wrapper around a single output AVStream
typedef struct OutputStream {
    AVStream *st;
    AVCodecContext *enc;
    /* pts of the next frame that will be generated */
    int64_t next_pts;
    int samples_count;
    AVFrame *frame;
    AVFrame *tmp_frame;
    AVPacket *tmp_pkt;
    float t, tincr, tincr2;
    struct SwsContext *sws_ctx;
    struct SwrContext *swr_ctx;
} OutputStream;
static void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt)
{
    AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;
#if 0    
    printf("pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
           av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
           av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
           av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
           pkt->stream_index);
#else
  time_base = time_base;           
#endif           
}
static int write_frame(AVFormatContext *fmt_ctx, AVCodecContext *c,
                       AVStream *st, AVFrame *frame, AVPacket *pkt)
{
    int ret;
  //printf("%s(%d) %X,%X,%X\n",__FILE__,__LINE__,c,frame,pkt);
    if(frame==0) return 1;
    // send the frame to the encoder
    ret = avcodec_send_frame(c, frame);
  //printf("%s(%d) %X,%X,%X\n",__FILE__,__LINE__,c,frame,pkt);
    if (ret < 0) {
        fprintf(stderr, "Error sending a frame to the encoder: %s\n",
                av_err2str(ret));
        exit(1);
    }
    while (ret >= 0) {
      //printf("%s(%d) %X,%X,%X\n",__FILE__,__LINE__,c,frame,pkt);
        ret = avcodec_receive_packet(c, pkt);
      //printf("write_frame %s(%d) %d ret=%d,%d,%d\n",__FILE__,__LINE__,frame->data[0][2],ret,AVERROR(EAGAIN),AVERROR_EOF);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            break;
        }    
        else if (ret < 0) {
            fprintf(stderr, "Error encoding a frame: %s\n", av_err2str(ret));
            exit(1);
        }
        /* rescale output packet timestamp values from codec to stream timebase */
        av_packet_rescale_ts(pkt, c->time_base, st->time_base);
        pkt->stream_index = st->index;
        /* Write the compressed frame to the media file. */
        log_packet(fmt_ctx, pkt);
        ret = av_interleaved_write_frame(fmt_ctx, pkt);
//      ret = av_write_frame(fmt_ctx, pkt);
        /* pkt is now blank (av_interleaved_write_frame() takes ownership of
         * its contents and resets pkt), so that no unreferencing is necessary.
         * This would be different if one used av_write_frame(). */
        if (ret < 0) {
            fprintf(stderr, "Error while writing output packet: %s\n", av_err2str(ret));
            exit(1);
        }
    }
    return ret == AVERROR_EOF ? 1 : 0;
}
/* Add an output stream. */
static void add_stream(OutputStream *ost, AVFormatContext *oc,
                       const AVCodec **codec,
                       enum AVCodecID codec_id)
{
    AVCodecContext *c;
    AVChannelLayout stereo_layout = AV_CHANNEL_LAYOUT_STEREO;
    int i;
    /* find the encoder */
    *codec = avcodec_find_encoder(codec_id);
    if (!(*codec)) {
        fprintf(stderr, "Could not find encoder for '%s'\n",
                avcodec_get_name(codec_id));
        exit(1);
    }
    ost->tmp_pkt = av_packet_alloc();
    if (!ost->tmp_pkt) {
        fprintf(stderr, "Could not allocate AVPacket\n");
        exit(1);
    }
    ost->st = avformat_new_stream(oc, NULL);
    if (!ost->st) {
        fprintf(stderr, "Could not allocate stream\n");
        exit(1);
    }
    ost->st->id = oc->nb_streams-1;
    c = avcodec_alloc_context3(*codec);
    if (!c) {
        fprintf(stderr, "Could not alloc an encoding context\n");
        exit(1);
    }
    ost->enc = c;
    switch ((*codec)->type) {
      case AVMEDIA_TYPE_AUDIO:
        c->sample_fmt  = (*codec)->sample_fmts ?
            (*codec)->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
        c->bit_rate    = 64000;
        c->sample_rate = 44100;
        if ((*codec)->supported_samplerates) {
            c->sample_rate = (*codec)->supported_samplerates[0];
            for (i = 0; (*codec)->supported_samplerates[i]; i++) {
                if ((*codec)->supported_samplerates[i] == 44100)
                    c->sample_rate = 44100;
            }
        }
        av_channel_layout_copy(&c->ch_layout, &stereo_layout);
//      av_channel_layout_copy(&c->ch_layout, &(AVChannelLayout)AV_CHANNEL_LAYOUT_STEREO);
        ost->st->time_base = (AVRational){ 1, c->sample_rate };
        break;
      case AVMEDIA_TYPE_VIDEO:
        c->codec_id = codec_id;

        c->bit_rate = 400000;
        /* Resolution must be a multiple of two. */
        #if 1
//      c->width    = GLOBAL_HEIGHT;   //2160; //352;
//      c->height   = GLOBAL_WIDTH;    //3840; //288;

//      c->width    = GLOBAL_WIDTH;    //2160; //352;
//      c->height   = GLOBAL_HEIGHT;   //3840; //288;

        c->width    = dec_ctx->width;    //2160; //352;
        c->height   = dec_ctx->height;   //3840; //288;

        #else
        c->width    = 352;
        c->height   = 288;
        #endif
        /* timebase: This is the fundamental unit of time (in seconds) in terms
         * of which frame timestamps are represented. For fixed-fps content,
         * timebase should be 1/framerate and timestamp increments should be
         * identical to 1. */
        ost->st->time_base = (AVRational){ 1, STREAM_FRAME_RATE };
        c->time_base       = ost->st->time_base;
        c->gop_size      = 12; /* emit one intra frame every twelve frames at most */
        c->pix_fmt       = STREAM_PIX_FMT;
        if (c->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
            /* just for testing, we also add B-frames */
            c->max_b_frames = 2;
        }
        if (c->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
            /* Needed to avoid using macroblocks in which some coeffs overflow.
             * This does not happen with normal video, it just happens here as
             * the motion of the chroma plane does not match the luma plane. */
            c->mb_decision = 2;
        }
        break;
    default:
        break;
    }
    /* Some formats want stream headers to be separate. */
    if (oc->oformat->flags & AVFMT_GLOBALHEADER)
        c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
}
/**************************************************************/
/* audio output */
static AVFrame *alloc_audio_frame(enum AVSampleFormat sample_fmt,
                                  const AVChannelLayout *channel_layout,
                                  int sample_rate, int nb_samples)
{
    AVFrame *frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Error allocating an audio frame\n");
        exit(1);
    }
    frame->format = sample_fmt;
    av_channel_layout_copy(&frame->ch_layout, channel_layout);
    frame->sample_rate = sample_rate;
    frame->nb_samples = nb_samples;

    if (nb_samples) {
        if (av_frame_get_buffer(frame, 0) < 0) {
            fprintf(stderr, "Error allocating an audio buffer\n");
            exit(1);
        }
    }
    return frame;
}
static void open_audio(AVFormatContext *oc, const AVCodec *codec,
                       OutputStream *ost, AVDictionary *opt_arg)
{
    AVCodecContext *c;
    int nb_samples;
    int ret;
    AVDictionary *opt = NULL;
    c = ost->enc;
    /* open it */
    av_dict_copy(&opt, opt_arg, 0);
    ret = avcodec_open2(c, codec, &opt);
    av_dict_free(&opt);
    if (ret < 0) {
        fprintf(stderr, "Could not open audio codec: %s\n", av_err2str(ret));
        exit(1);
    }
    /* init signal generator */
    ost->t     = 0;
    ost->tincr = 2 * M_PI * 110.0 / c->sample_rate;
    /* increment frequency by 110 Hz per second */
    ost->tincr2 = 2 * M_PI * 110.0 / c->sample_rate / c->sample_rate;
    if (c->codec->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE)
        nb_samples = 10000;
    else
        nb_samples = c->frame_size;
    ost->frame     = alloc_audio_frame(c->sample_fmt, &c->ch_layout,
                                       c->sample_rate, nb_samples);
    ost->tmp_frame = alloc_audio_frame(AV_SAMPLE_FMT_S16, &c->ch_layout,
                                       c->sample_rate, nb_samples);
    /* copy the stream parameters to the muxer */
    ret = avcodec_parameters_from_context(ost->st->codecpar, c);
    if (ret < 0) {
        fprintf(stderr, "Could not copy the stream parameters\n");
        exit(1);
    }
    /* create resampler context */
    ost->swr_ctx = swr_alloc();
    if (!ost->swr_ctx) {
        fprintf(stderr, "Could not allocate resampler context\n");
        exit(1);
    }
    /* set options */
    av_opt_set_chlayout  (ost->swr_ctx, "in_chlayout",       &c->ch_layout,      0);
    av_opt_set_int       (ost->swr_ctx, "in_sample_rate",     c->sample_rate,    0);
    av_opt_set_sample_fmt(ost->swr_ctx, "in_sample_fmt",      AV_SAMPLE_FMT_S16, 0);
    av_opt_set_chlayout  (ost->swr_ctx, "out_chlayout",      &c->ch_layout,      0);
    av_opt_set_int       (ost->swr_ctx, "out_sample_rate",    c->sample_rate,    0);
    av_opt_set_sample_fmt(ost->swr_ctx, "out_sample_fmt",     c->sample_fmt,     0);
    /* initialize the resampling context */
    if ((ret = swr_init(ost->swr_ctx)) < 0) {
        fprintf(stderr, "Failed to initialize the resampling context\n");
        exit(1);
    }
}
/* Prepare a 16 bit dummy audio frame of 'frame_size' samples and
 * 'nb_channels' channels. */
static AVFrame *get_audio_frame(OutputStream *ost)
{
    AVFrame *frame = ost->tmp_frame;
    int j, i, v;
    int16_t *q = (int16_t*)frame->data[0];
    /* check if we want to generate more frames */
    if (av_compare_ts(ost->next_pts, ost->enc->time_base,
                      STREAM_DURATION, (AVRational){ 1, 1 }) > 0)
        return NULL;
    for (j = 0; j <frame->nb_samples; j++) {
        v = (int)(sin(ost->t) * 10000);
        for (i = 0; i < ost->enc->ch_layout.nb_channels; i++)
            *q++ = v;
        ost->t     += ost->tincr;
        ost->tincr += ost->tincr2;
    }
    frame->pts = ost->next_pts;
    ost->next_pts  += frame->nb_samples;
    return frame;
}
/*
 * encode one audio frame and send it to the muxer
 * return 1 when encoding is finished, 0 otherwise
 */
static int write_audio_frame(AVFormatContext *oc, OutputStream *ost)
{
    AVCodecContext *c;
    AVFrame *frame;
    int ret;
    int dst_nb_samples;
    c = ost->enc;
    frame = get_audio_frame(ost);
    if (frame) {
        /* convert samples from native format to destination codec format, using the resampler */
        /* compute destination number of samples */
        dst_nb_samples = av_rescale_rnd(swr_get_delay(ost->swr_ctx, c->sample_rate) + frame->nb_samples,
                                        c->sample_rate, c->sample_rate, AV_ROUND_UP);
        av_assert0(dst_nb_samples == frame->nb_samples);
        /* when we pass a frame to the encoder, it may keep a reference to it
         * internally;
         * make sure we do not overwrite it here
         */
        ret = av_frame_make_writable(ost->frame);
        if (ret < 0)
            exit(1);
        /* convert to destination format */
        ret = swr_convert(ost->swr_ctx,
                          ost->frame->data, dst_nb_samples,
                          (const uint8_t **)frame->data, frame->nb_samples);
        if (ret < 0) {
            fprintf(stderr, "Error while converting\n");
            exit(1);
        }
        frame = ost->frame;

        frame->pts = av_rescale_q(ost->samples_count, (AVRational){1, c->sample_rate}, c->time_base);
        ost->samples_count += dst_nb_samples;
    }
    return write_frame(oc, c, ost->st, frame, ost->tmp_pkt);
}
/**************************************************************/
/* video output */
static AVFrame *alloc_frame(enum AVPixelFormat pix_fmt, int width, int height)
{
    AVFrame *frame;
    int ret;
    frame = av_frame_alloc();
    if (!frame)
        return NULL;
    frame->format = pix_fmt;
    frame->width  = width;
    frame->height = height;
    /* allocate the buffers for the frame data */
    ret = av_frame_get_buffer(frame, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate frame data.\n");
        exit(1);
    }
    return frame;
}
static void open_video(AVFormatContext *oc, const AVCodec *codec,
                       OutputStream *ost, AVDictionary *opt_arg)
{
    int ret;
    AVCodecContext *c = ost->enc;
    AVDictionary *opt = NULL;
    av_dict_copy(&opt, opt_arg, 0);
    /* open the codec */
    ret = avcodec_open2(c, codec, &opt);
    av_dict_free(&opt);
    if (ret < 0) {
        fprintf(stderr, "Could not open video codec: %s\n", av_err2str(ret));
        exit(1);
    }
    /* allocate and init a re-usable frame */
    ost->frame = alloc_frame(c->pix_fmt, c->width, c->height);
    if (!ost->frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    /* If the output format is not YUV420P, then a temporary YUV420P
     * picture is needed too. It is then converted to the required
     * output format. */
    ost->tmp_frame = NULL;
    if (c->pix_fmt != AV_PIX_FMT_YUV420P) {
        ost->tmp_frame = alloc_frame(AV_PIX_FMT_YUV420P, c->width, c->height);
        if (!ost->tmp_frame) {
            fprintf(stderr, "Could not allocate temporary video frame\n");
            exit(1);
        }
    }
    /* copy the stream parameters to the muxer */
    ret = avcodec_parameters_from_context(ost->st->codecpar, c);
    if (ret < 0) {
        fprintf(stderr, "Could not copy the stream parameters\n");
        exit(1);
    }
} 
#include <stdbool.h>
#define MAX_JUMP 2
bool marginal(int x,int y);
int HXYV[60][60];
int HXYY[30][30];
int HXYU[30][30];
int EX[3840];
int EY[3840];
void calc_64x36(AVFrame *pict, int frame_index,
                int width, int height);              
//娩絫,,狠翴,癬翴,沧翴
#define edgeSpace      1               //オ娩絫ぃ衡
#define maxDeltaSpace  1               //程ぃ琌絬フ
#define minPointNrLine 6               //程ぶ絬翴计
typedef struct STLINE {
  short c0;  //0..3840-1 =coordinate
  short c1;  //0..65535
  int w;     //翴计
} ST_LINE;
typedef struct STLINELIST {
  ST_LINE L;
  struct STLINELIST *next;
} ST_LINE_LIST;
ST_LINE_LIST *lnHead=NULL;
void initLineList(int width,int height);
void freeLineList();
#define MAX_JUMP_LINE 12
bool marginalLine(int x,int y);
//(x0,y0 ...x,y,... x1,y1)
#define ratioS  1.33333
#define ratioT -1.33333
#define MAX_JUMP_SLOPE 12
typedef struct STSLOPE { //Slope A,B
  short y0;
  short c0;  //0..3840-1 =coordinate
  short c1;  //0..65535
  int w;     //翴计
} ST_SLOPE;
typedef struct STSLOPELIST {
  ST_SLOPE s;
  struct STSLOPELIST *next;
} ST_SLOPE_LIST;
ST_SLOPE_LIST *slHead=NULL;
void initSlopeList(int width,int height);
void freeSlopeList();
#define sign(x) (x>0?1:-1)
#define maxDeltaSlopeSpace 1
bool marginalSlope(int x,int y,int ystart,float ratio,int width,int height);
//XYZST
//Xfi X frame index
//PQR R rect
int wp,
    Xfi,XposLine,Xc0,Xc1,Xw,
    Yfi,YposLine,Yc0,Yc1,Yw,
    Zfi,ZposLine,Zc0,Zc1,Zw,
    Sfi,SposSlope,Sc0,Sc1,Sy0,Sw,
    Tfi,TposSlope,Tc0,Tc1,Ty0,Tw
    ;
void FindXYZSTLineList(AVFrame *pict, int frame_index,
                    int width, int height);
int Xmax,Xleft,Xright,Ymax,Yup,Ydown;
//#define MAXNR_PT 64
//int HX[GLOBAL_WIDTH],HY[GLOBAL_HEIGHT];
int HX[GLOBAL_HEIGHT/2],HY[GLOBAL_WIDTH/2];
void calc_histogram(AVFrame *pict, int frame_index,
                    int width, int height);
void calc_edge(int frame_index, int width, int height);
/* Prepare a dummy image. */
void fill_yuv_image(AVFrame *pict, int frame_index,
                    int width, int height);
static AVFrame *get_video_frame(OutputStream *ost)
{
    AVCodecContext *c = ost->enc;
    printf("%s(%d)\n",__FILE__,__LINE__);
    /* check if we want to generate more frames */
    if (av_compare_ts(ost->next_pts, c->time_base,
                      STREAM_DURATION, (AVRational){ 1, 1 }) > 0)
        return NULL;
    /* when we pass a frame to the encoder, it may keep a reference to it
     * internally; make sure we do not overwrite it here */
    if (av_frame_make_writable(ost->frame) < 0)
        exit(1);
    if (c->pix_fmt != AV_PIX_FMT_YUV420P) {
        /* as we only generate a YUV420P picture, we must convert it
         * to the codec pixel format if needed */
        if (!ost->sws_ctx) {
            ost->sws_ctx = sws_getContext(c->width, c->height,
                                          AV_PIX_FMT_YUV420P,
                                          c->width, c->height,
                                          c->pix_fmt,
                                          SCALE_FLAGS, NULL, NULL, NULL);
            if (!ost->sws_ctx) {
                fprintf(stderr,
                        "Could not initialize the conversion context\n");
                exit(1);
            }
        }
        printf("%s(%d) (%d,%d)\n",__FILE__,__LINE__,c->width, c->height);
        fill_yuv_image(ost->tmp_frame, ost->next_pts, c->width, c->height);
        sws_scale(ost->sws_ctx, (const uint8_t * const *) ost->tmp_frame->data,
                  ost->tmp_frame->linesize, 0, c->height, ost->frame->data,
                  ost->frame->linesize);
    } else {
        printf("%s(%d)%d,(%d,%d)\n",__FILE__,__LINE__,ost->next_pts,c->width, c->height);
#if 1
        calc_histogram(ost->frame, ost->next_pts, c->width, c->height);
        calc_64x36(ost->frame, ost->next_pts, c->width, c->height);
        initLineList(c->width, c->height);
        initSlopeList(c->width, c->height);
        FindXYZSTLineList(ost->frame, ost->next_pts, c->width, c->height);
#endif        
        fill_yuv_image(ost->frame, ost->next_pts, c->width, c->height);
#if 0
        freeLineList();
        freeSlopeList();
#endif        
    }
    ost->frame->pts = ost->next_pts++;
    return ost->frame;
}

//unsigned char *filt_diffnow_buffer=NULL;
//unsigned char *filt_before_buffer=NULL;
int rn[3][3],gn[3][3],bn[3][3];
int rb[3][3],gb[3][3],bb[3][3];
int re,ge,be,ra,ga,ba;
extern void calc_matrix(int x,int y);
void copyFrame_now(int frame_index) {
  int x,y,x2,y2,x0,y0,Y,U,V;
  if(Ydiffnow == NULL) {
    frameWidth    = filt_frame->width;
    frameHeight   = filt_frame->height;
    Ylinesize =  filt_frame->linesize[0];
    Ulinesize =  filt_frame->linesize[1];
    Vlinesize =  filt_frame->linesize[2];
    Ynow = (unsigned char *)malloc(Ylinesize*frameHeight);
    Unow = (unsigned char *)malloc(Ulinesize*frameHeight);
    Vnow = (unsigned char *)malloc(Vlinesize*frameHeight);
    Ydiffnow = (unsigned char *)malloc(Ylinesize*frameHeight);
    Udiffnow = (unsigned char *)malloc(Ulinesize*frameHeight);
    Vdiffnow = (unsigned char *)malloc(Vlinesize*frameHeight);
    Yref = (unsigned char *)malloc(Ylinesize*frameHeight);
    Uref = (unsigned char *)malloc(Ulinesize*frameHeight);
    Vref = (unsigned char *)malloc(Vlinesize*frameHeight);
    Rnow = (unsigned char *)malloc(Ylinesize*frameHeight);
    Gnow = (unsigned char *)malloc(Ylinesize*frameHeight);
    Bnow = (unsigned char *)malloc(Ylinesize*frameHeight);
    printf("%s(%d),(%4d,%4d),(%4d,%4d,%4d)\n",__FILE__,__LINE__,
           frameWidth,frameHeight,
           Ylinesize,Ulinesize,Vlinesize);
           
  }
  if(frame_index==251) {
    printf("%s(%d),(%4d,%4d),(%4d,%4d,%4d)\n",__FILE__,__LINE__,
           frameWidth,frameHeight,
           Ylinesize,Ulinesize,Vlinesize);
  }
#if 1
  for (y = 0; y < filt_frame->height; y++) {
    y2 = y/2;
    for (x = 0; x < filt_frame->width; x++) {

      calc_matrix(x,y);     
      x2 = x/2;
         
      Ynow[y * Ylinesize + x]  = filt_frame->data[0][y * Ylinesize + x];
      Unow[y2* Ulinesize + x2] = filt_frame->data[1][y2* Ulinesize + x2];
      Vnow[y2* Vlinesize + x2] = filt_frame->data[2][y2* Vlinesize + x2];
//    if(ga > 128 && ba > 32) {
//    if(ga > 128) {
//    if(ge > 16 && ba > 16) {
      if(ge > 16) {
//      printf("(%5d,%5d,%5d)\n",re,ge,be);
//    if(r[1][1]<128  && g[1][1] > 128) {
        Ydiffnow[y * Ylinesize + x]  = BLACKY;
        Udiffnow[y2* Ulinesize + x2] = BLACKU;
        Vdiffnow[y2* Vlinesize + x2] = BLACKV;
//      printf("%s(%d),%4d,%4d,%5d,%5d,%5d,%5d,%5d,%5d\n",__FILE__,__LINE__,y,x,Y,U,V,r,g,b);
      }
      else {
        Ydiffnow[y * Ylinesize + x]  = WHITEY;
        Udiffnow[y2* Ulinesize + x2] = WHITEU;
        Vdiffnow[y2* Vlinesize + x2] = WHITEV;
//      printf("%s(%d),%4d,%4d,%5d,%5d,%5d,%5d,%5d,%5d\n",__FILE__,__LINE__,y,x,Y,U,V,r,g,b);
      }
    }
  }    
      if(frame_index==251) {
        printf("%s(%d),(%4d,%4d),(%4d,%4d)\n",__FILE__,__LINE__,
               filt_frame->height,filt_frame->width,
               y,x);
      }
#endif  
}

void copyFramebefore() {
  int x,y,x2,y2;
  if(Ybefore == NULL) {
    frameWidth    = filt_frame->width;
    frameHeight   = filt_frame->height;
    Ylinesize =  filt_frame->linesize[0];
    Ulinesize =  filt_frame->linesize[1];
    Vlinesize =  filt_frame->linesize[2];   
    Ybefore = (unsigned char *)malloc(Ylinesize*frameHeight);
    Ubefore = (unsigned char *)malloc(Ulinesize*frameHeight);
    Vbefore = (unsigned char *)malloc(Vlinesize*frameHeight);
    Ybeforediff = (unsigned char *)malloc(Ylinesize*frameHeight);
    Ubeforediff = (unsigned char *)malloc(Ulinesize*frameHeight);
    Vbeforediff = (unsigned char *)malloc(Vlinesize*frameHeight);
    
    Rbefore = (unsigned char *)malloc(Ylinesize*frameHeight);
    Gbefore = (unsigned char *)malloc(Ylinesize*frameHeight);
    Bbefore = (unsigned char *)malloc(Ylinesize*frameHeight);
  }
#if 0
  for (y = 0; y < filt_frame->height; y++) {
    y2 = y/2;
    for (x = 0; x < filt_frame->width; x++) {
      x2 = x/2;
      Ybefore[y  * Ylinesize + x] =  Ynow[y * Ylinesize + x];
      Ubefore[y2 * Ulinesize + x2] = Unow[y2* Ulinesize + x2];
      Vbefore[y2 * Vlinesize + x2] = Vnow[y2* Vlinesize + x2];
      Ybeforediff[y  * Ylinesize + x] =  Ydiffnow[y * Ylinesize + x];
      Ubeforediff[y2 * Ulinesize + x2] = Udiffnow[y2* Ulinesize + x2];
      Vbeforediff[y2 * Vlinesize + x2] = Vdiffnow[y2* Vlinesize + x2];
      Rbefore[y  * Ylinesize + x] =  Rnow[y * Ylinesize + x];
      Gbefore[y  * Ylinesize + x] =  Gnow[y * Ylinesize + x];
      Bbefore[y  * Ylinesize + x] =  Bnow[y * Ylinesize + x];
    }  
  }  
#else
//printf("%s(%d) %4d\n",__FILE__,__LINE__,sizeof(Ybefore));exit(0);
  memcpy(Ybefore,Ynow,Ylinesize*frameHeight);
  memcpy(Ubefore,Unow,Ulinesize*frameHeight);
  memcpy(Vbefore,Vnow,Vlinesize*frameHeight);
  memcpy(Ybeforediff,Ydiffnow,Ylinesize*frameHeight);
  memcpy(Ubeforediff,Udiffnow,Ulinesize*frameHeight);
  memcpy(Vbeforediff,Vdiffnow,Vlinesize*frameHeight);
  memcpy(Rbefore,Rnow,Ylinesize*frameHeight);
  memcpy(Gbefore,Gnow,Ylinesize*frameHeight);
  memcpy(Bbefore,Bnow,Ylinesize*frameHeight);
#endif  
}

/*
 * encode one video frame and send it to the muxer
 * return 1 when encoding is finished, 0 otherwise
 */
static int write_video_frame(AVFormatContext *oc, OutputStream *ost)
{
    return write_frame(oc, ost->enc, ost->st, get_video_frame(ost), ost->tmp_pkt);
}
static void close_stream(AVFormatContext *oc, OutputStream *ost)
{
    avcodec_free_context(&ost->enc);
    av_frame_free(&ost->frame);
    av_frame_free(&ost->tmp_frame);
    av_packet_free(&ost->tmp_pkt);
    sws_freeContext(ost->sws_ctx);
    swr_free(&ost->swr_ctx);
}
//mux end///////////////////////////////////////////////////////////////////////
static int open_input_file(const char *filename)
{
    const AVCodec *dec;
    int ret;
    if ((ret = avformat_open_input(&fmt_ctx, filename, NULL, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
        return ret;
    }
    if ((ret = avformat_find_stream_info(fmt_ctx, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
        return ret;
    }
    /* select the video stream */
    ret = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &dec, 0);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot find a video stream in the input file\n");
        return ret;
    }
    video_stream_index = ret;
    /* create decoding context */
    dec_ctx = avcodec_alloc_context3(dec);
    if (!dec_ctx)
        return AVERROR(ENOMEM);
    avcodec_parameters_to_context(dec_ctx, fmt_ctx->streams[video_stream_index]->codecpar);
    /* init the video decoder */
    if ((ret = avcodec_open2(dec_ctx, dec, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open video decoder\n");
        return ret;
    }
    return 0;
}      
static int init_filters(const char *filters_descr)
{
    char args[512];
    int ret = 0;
    const AVFilter *buffersrc  = avfilter_get_by_name("buffer");
    const AVFilter *buffersink = avfilter_get_by_name("buffersink");
    AVFilterInOut *outputs = avfilter_inout_alloc();
    AVFilterInOut *inputs  = avfilter_inout_alloc();
    AVRational time_base = fmt_ctx->streams[video_stream_index]->time_base;
//  enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_GRAY8, AV_PIX_FMT_NONE };
//  enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_RGB24, AV_PIX_FMT_NONE };
//  enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_MONOWHITE , AV_PIX_FMT_NONE };
//  enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_MONOBLACK , AV_PIX_FMT_NONE };
    enum AVPixelFormat pix_fmts[] = { STREAM_PIX_FMT, AV_PIX_FMT_NONE };
    filter_graph = avfilter_graph_alloc();
    if (!outputs || !inputs || !filter_graph) {
        ret = AVERROR(ENOMEM);
        goto end;
    }
    /* buffer video source: the decoded frames from the decoder will be inserted here. */
    snprintf(args, sizeof(args),
            "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
            dec_ctx->width, dec_ctx->height, dec_ctx->pix_fmt,
            time_base.num, time_base.den,
            dec_ctx->sample_aspect_ratio.num, dec_ctx->sample_aspect_ratio.den);
    ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in",
                                       args, NULL, filter_graph);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot create buffer source\n");
        goto end;
    }
    /* buffer video sink: to terminate the filter chain. */
    ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out",
                                       NULL, NULL, filter_graph);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot create buffer sink\n");
        goto end;
    }
//  ret = av_opt_set_int_list(buffersink_ctx, "pix_fmts", pix_fmts,
//                            AV_PIX_FMT_NONE, AV_OPT_SEARCH_CHILDREN);
    ret = av_opt_set_int_list(buffersink_ctx, "pix_fmts", pix_fmts,
                              AV_PIX_FMT_NONE, AV_OPT_SEARCH_CHILDREN);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot set output pixel format\n");
        goto end;
    }
    /*
     * Set the endpoints for the filter graph. The filter_graph will
     * be linked to the graph described by filters_descr.
     */
    /*
     * The buffer source output must be connected to the input pad of
     * the first filter described by filters_descr; since the first
     * filter input label is not specified, it is set to "in" by
     * default.
     */
    outputs->name       = av_strdup("in");
    outputs->filter_ctx = buffersrc_ctx;
    outputs->pad_idx    = 0;
    outputs->next       = NULL;
    /*
     * The buffer sink input must be connected to the output pad of
     * the last filter described by filters_descr; since the last
     * filter output label is not specified, it is set to "out" by
     * default.
     */
    inputs->name       = av_strdup("out");
    inputs->filter_ctx = buffersink_ctx;
    inputs->pad_idx    = 0;
    inputs->next       = NULL;
    if ((ret = avfilter_graph_parse_ptr(filter_graph, filters_descr,
                                    &inputs, &outputs, NULL)) < 0)
        goto end;

    if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0)
        goto end;
end:
    avfilter_inout_free(&inputs);
    avfilter_inout_free(&outputs);
    return ret;
}
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    for (i = size-1; i >= 0; i--) {
        byte = b[i];
        for (j = 7; j >= 0; j--) {
            if(byte & 0x80) putchar('1');
            else     putchar(' ');
            byte = byte << 1;
        }
    }
}

#if 0
static void display_frame(const AVFrame *frame, AVRational time_base)
{
    int x, y;
    uint8_t *p0, *p;
    int64_t delay;
    if (frame->pts != AV_NOPTS_VALUE) {
        if (last_pts != AV_NOPTS_VALUE) {
            /* sleep roughly the right amount of time;
             * usleep is in microseconds, just like AV_TIME_BASE. */
            delay = av_rescale_q(frame->pts - last_pts,
                                 time_base, AV_TIME_BASE_Q);
            if (delay > 0 && delay < 1000000)
                usleep(delay);
        }
        last_pts = frame->pts;
    }
    /* Trivial ASCII grayscale display. */
    p0 = frame->data[0];
    puts("\033c"); 
//    printf("h=%d,w=%d,L=%d,%d,%d,%d\n",
//    frame->height,frame->width,frame->linesize[0],
//    *(p0),*(p0+1),*(p0+2));
//  exit(0);
    for (y = 0; y < frame->height; y++) {
        p = p0;
        for (x = 0; x < frame->width/8; x++) {
//          putchar("0123456789"[*(p) / 26]);
//          putchar("X         "[*(p) / 26]);
            printBits(1, p);
			p+=1;
		}	
        putchar('\n');
        p0 += frame->linesize[0];
    }
    fflush(stdout);
}
#endif

int sdl_main(int argc, char *argv[]);

#define XLENGTH 3840
int UEY[XLENGTH];
int DEY[XLENGTH];
int main(int argc, char **argv)
{
    time_t now0,now1;
    int ret;
    AVPacket *packet;
    AVFrame *frame;
//  AVFrame *filt_frame;
    int ilooptime=0;
 //mux bein
    OutputStream video_st = { 0 }, audio_st = { 0 };
    const AVOutputFormat *fmt;
    const char *filename;
    AVFormatContext *oc;
    const AVCodec *audio_codec, *video_codec;
//  int ret;
    int have_video = 0, have_audio = 0;
    int encode_video = 0, encode_audio = 0;
    AVDictionary *opt = NULL;
    int i;    
//mux end
    now0 = time(NULL);
    memset(UEY,0,XLENGTH*4);
    memset(DEY,0,XLENGTH*4);
    frame = av_frame_alloc();
    filt_frame   = av_frame_alloc();

    packet = av_packet_alloc();
    if (!frame || !filt_frame || !packet) {
        fprintf(stderr, "Could not allocate frame or packet\n");
        exit(1);
    }
    if ((ret = open_input_file(argv[1])) < 0)
        goto end;
//  sprintf(filter_descr,"scale=%d:%d,transpose=clock",GLOBAL_WIDTH,GLOBAL_HEIGHT);
//  sprintf(filter_descr,"scale=%d:%d",GLOBAL_WIDTH,GLOBAL_HEIGHT);
    sprintf(filter_descr,"scale=iw:ih");
    if ((ret = init_filters(filter_descr)) < 0)
        goto end;
    printf("video_size=%dx%d:pix_fmt=%d:pixel_aspect=%d/%d\n",
            dec_ctx->width, dec_ctx->height, dec_ctx->pix_fmt,
            dec_ctx->sample_aspect_ratio.num, dec_ctx->sample_aspect_ratio.den);
//mux begin    
        if (argc < 3) {
        printf("usage: %s input_file output_file looptime flag\n"
               "API example program to output a media file with libavformat.\n"
               "This program generates a synthetic audio and video stream, encodes and\n"
               "muxes them into a file named output_file.\n"
               "The output format is automatically guessed according to the file extension.\n"
               "Raw images can also be output by using '%%d' in the filename.\n"
               "\n", argv[0]);
        return 1;
    }

    filename = argv[2];
    for (i = 3; i+1 < argc; i+=2) {
        if (!strcmp(argv[i], "-flags") || !strcmp(argv[i], "-fflags"))
            av_dict_set(&opt, argv[i]+1, argv[i+1], 0);
    }

    /* allocate the output media context */
    avformat_alloc_output_context2(&oc, NULL, NULL, filename);
    if (!oc) {
        printf("Could not deduce output format from file extension: using MPEG.\n");
        avformat_alloc_output_context2(&oc, NULL, "mpeg", filename);
    }
    if (!oc)
        return 1;

    fmt = oc->oformat;

    /* Add the audio and video streams using the default format codecs
     * and initialize the codecs. */
    if (fmt->video_codec != AV_CODEC_ID_NONE) {
        add_stream(&video_st, oc, &video_codec, fmt->video_codec);
        have_video = 1;
        encode_video = 1;
    }
#if 0    
//ノ硂碭︽穦Τ羘
    if (fmt->audio_codec != AV_CODEC_ID_NONE) {
        add_stream(&audio_st, oc, &audio_codec, fmt->audio_codec);
        have_audio = 1;
        encode_audio = 1;
    }
#endif    

    /* Now that all the parameters are set, we can open the audio and
     * video codecs and allocate the necessary encode buffers. */
    if (have_video)
        open_video(oc, video_codec, &video_st, opt);

    if (have_audio)
        open_audio(oc, audio_codec, &audio_st, opt);

    av_dump_format(oc, 0, filename, 1);
    /* open the output file, if needed */
    if (!(fmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&oc->pb, filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            fprintf(stderr, "Could not open '%s': %s\n", filename,
                    av_err2str(ret));
            return 1;
        }
    }
    /* Write the stream header, if any. */
    ret = avformat_write_header(oc, &opt);
    if (ret < 0) {
        fprintf(stderr, "Error occurred when opening output file: %s\n",
                av_err2str(ret));
        return 1;
    }
//mux end
   if (argc<3 ||argc>4) {
        fprintf(stderr, "Usage: %s input_file output_file looptime\n", argv[0]);
        exit(1);
    }
    else if(argc==4) {
        ilooptime=atoi(argv[3]);
    }
    ilooptime=ilooptime; 
/*	
  {  
    AVFormatContext *fmt_ctx = NULL;
    const AVDictionaryEntry *tag = NULL;
	 if ((ret = avformat_open_input(&fmt_ctx, argv[1], NULL, NULL)))
      return ret;
    if ((ret = avformat_find_stream_info(fmt_ctx, NULL)) < 0) {
      av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
      return ret;
    }
    while ((tag = av_dict_iterate(fmt_ctx->metadata, tag)))
      printf("%s=%s\n", tag->key, tag->value);
    avformat_close_input(&fmt_ctx);
  }  
  {
    double theta = 0.0;
    int32_t *displaymatrix = NULL;
    AVFrameSideData *sd = av_frame_get_side_data(frame, AV_FRAME_DATA_DISPLAYMATRIX);
    if (sd)
      displaymatrix = (int32_t *)sd->data;
    if (!displaymatrix)
      displaymatrix = (int32_t *)av_stream_get_side_data(is->video_st, AV_PKT_DATA_DISPLAYMATRIX, NULL);
  }  
  {
    int len =0;
    int iloopcount = 0;
    AVProbeData probeData = {0};
	 probeData.buf_size = len < 4096 ? len : 4096;
	 probeData.filename = "stream";
	 probeData.buf = av_mallocz(probeData.buf_size + AVPROBE_PADDING_SIZE);
	 memcpy(probeData.buf, lpData, probeData.buf_size);
  }
  */
//mux begin

  sdl_main(argc, argv);

//mux end
    /* read all packets */
    while (1) {
        if ((ret = av_read_frame(fmt_ctx, packet)) < 0)
            break;
        if (packet->stream_index == video_stream_index) {
            ret = avcodec_send_packet(dec_ctx, packet);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Error while sending a packet to the decoder\n");
                break;
            }
            while (ret >= 0) {
                ret = avcodec_receive_frame(dec_ctx, frame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    break;
                } else if (ret < 0) {
                    av_log(NULL, AV_LOG_ERROR, "Error while receiving a frame from the decoder\n");
                    goto end;
                }
                frame->pts = frame->best_effort_timestamp;
                /* push the decoded frame into the filtergraph */
                if (av_buffersrc_add_frame_flags(buffersrc_ctx, frame, AV_BUFFERSRC_FLAG_KEEP_REF) < 0) {
                    av_log(NULL, AV_LOG_ERROR, "Error while feeding the filtergraph\n");
                    break;
                }
                /* pull filtered frames from the filtergraph */
                while (1) {
                    ret = av_buffersink_get_frame(buffersink_ctx, filt_frame);
                    printf("%s(%d),%3d,%3d,%3d,%3d\n",__FILE__,__LINE__,
                      frame->data[0][2],filt_frame->data[0][2],
                      encode_video,encode_audio);
                    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                      printf("%s(%d),%3d,%3d\n",__FILE__,__LINE__,
                        encode_video,encode_audio);                   
                      break;
                    } 
                    if (ret < 0) {
                      printf("%s(%d),%3d,%3d,%3d\n",__FILE__,__LINE__,
                             filt_frame->data[0][2],encode_video,encode_audio);                   
                      goto end;
                    }  
//                  display_frame(filt_frame, buffersink_ctx->inputs[0]->time_base);
#if 1
                    /* select the stream to encode */
                    if (encode_video &&
                      (!encode_audio || av_compare_ts(video_st.next_pts, video_st.enc->time_base,
                        audio_st.next_pts, audio_st.enc->time_base) <= 0)) {
                        printf("%s(%d),%3d\n",__FILE__,__LINE__,video_st.next_pts);
                        copyFrame_now(video_st.next_pts);
                      //calc_edge(video_st.next_pts,video_st.enc->width, video_st.enc->height);
                        encode_video = !write_video_frame(oc, &video_st);
                        if(encode_video==0 && !encode_audio) 
                          goto end_video;
                    //  encode_audio = !write_audio_frame(oc, &audio_st);
                    } else {
                        printf("else encode_audio %s(%d)\n",__FILE__,__LINE__);
                        encode_audio = !write_audio_frame(oc, &audio_st);
                    }                 
                    printf("%s(%d),%3d\n",__FILE__,__LINE__,video_st.next_pts);
                    copyFramebefore();
#endif
//                  av_frame_unref(filt_frame);
                }
//              av_frame_unref(frame);
            }
        }
//      av_packet_unref(packet);
    }
//mux begin
#if 0
    while (encode_video || encode_audio) {
        /* select the stream to encode */
        if (encode_video &&
            (!encode_audio || av_compare_ts(video_st.next_pts, video_st.enc->time_base,
                                            audio_st.next_pts, audio_st.enc->time_base) <= 0)) {
            encode_video = !write_video_frame(oc, &video_st);
            printf("line %d\n",__LINE__);
        } else {
            encode_audio = !write_audio_frame(oc, &audio_st);
        }
    }
#endif
end_video:
    printf("%s(%d)\n",__FILE__,__LINE__);
    av_write_trailer(oc);
    /* Close each codec. */
    if (have_video)
        close_stream(oc, &video_st);
    if (have_audio)
        close_stream(oc, &audio_st);
    if (!(fmt->flags & AVFMT_NOFILE))
        /* Close the output file. */
        avio_closep(&oc->pb);
    /* free the stream */
    avformat_free_context(oc);
    if (ret < 0 && ret != AVERROR_EOF) {
        fprintf(stderr, "Error occurred: %s\n", av_err2str(ret));
        exit(1);
    }    
//mux end
end:
#if 1
    av_frame_unref(filt_frame);
    av_frame_unref(frame);
    av_packet_unref(packet);   
//  free(Ybefore);free(Ubefore);free(Vbefore);
//  free(Ydiffnow);free(Udiffnow);free(Vdiffnow);
#endif
//  avfilter_graph_free(&filter_graph);  //2025/4/1 by calc_matrix ?
    avcodec_free_context(&dec_ctx);
    avformat_close_input(&fmt_ctx);
    av_frame_free(&frame);
    av_frame_free(&filt_frame);
    av_packet_free(&packet);
    if (ret < 0 && ret != AVERROR_EOF) {
        fprintf(stderr, "Error occurred: %s\n", av_err2str(ret));
        exit(1);
    }
    now1 = time(NULL);
    printf("diff=%lu\n",now1-now0);
    exit(0);
}

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
void __assert_func(const char *file, int line, const char *func, const char *failedexpr) {
    // You can customize this to print to stderr or a logging system
    // Or just leave it empty if you want to silence the error
};

//#include <stdlib.h> 
//#include <string.h>
#include <wchar.h>
#include <math.h>
#include <cstring>
#include <vector>
#define MAX_LINE_LENGTH 256
struct Button {
  SDL_Rect rect;
  bool isPressed;
  char fstr[MAX_LINE_LENGTH];
};
bool isMouseOver(int mx, int my, const SDL_Rect& rect) {
  return (mx >= rect.x && mx <= rect.x + rect.w &&
          my >= rect.y && my <= rect.y + rect.h);
}
#include <unistd.h>
SDL_Point pt[3][20];

SDL_Point ptClick;
int nPt[3];
FILE *ptFile=NULL;
char fDirectory[MAX_LINE_LENGTH];
char ptfname[MAX_LINE_LENGTH];
char picSN[20];
int NpicSN;
char buffer[MAX_LINE_LENGTH];
void initfDirectory(void) {
  int len;
  strcpy(ptfname,"data.txt"); 
  strcpy(fDirectory,"../VID");
  if (access(fDirectory, F_OK) == 0) {
  }
  else {
    fDirectory[0]=0;
  }
  if (access(ptfname, F_OK) == 0) {
    ptFile = fopen(ptfname,"rt");
    // Read line-by-line until fgets returns NULL
    while (fgets(buffer, sizeof(buffer), ptFile) != NULL) {
      len=strlen(buffer);
      if(buffer[len-1]==0x0A) buffer[len-1]=0;
      if (access(buffer, F_OK) == 0) {
        strcpy(fDirectory,buffer);
        break;
      }  
    }
    if(ptFile!=NULL) fclose(ptFile); ptFile=NULL;
  }
}
void initpicSN(void) {
  int i,x,j;
  char snstr[6];
  if(fDirectory[0]==0) {
    printf("initpicSN no Directory\n");
    return;
  }
  sprintf(ptfname,"%s/data.txt",fDirectory);  
  if (access(ptfname, F_OK) == 0) {
    // file exists
    ptFile = fopen(ptfname,"rt");

    // Read line-by-line until fgets returns NULL
    i=0;
    NpicSN=0;
    while (fgets(buffer, sizeof(buffer), ptFile) != NULL) {
      j=strlen(buffer);if(buffer[j-1]==0x0A) buffer[j-1]=0x0;
      x=atoi(buffer);
      sprintf(snstr,"%d",x);
//    printf("serial=[%4d],[%s],%02X\n",x,buffer,buffer[j-1]);
      if (!memcmp(buffer,snstr,strlen(buffer))) {
        picSN[i]=x;
        i++;
        NpicSN=i;
      }
      if(i>15) break;
    }
    if(ptFile!=NULL) fclose(ptFile); ptFile=NULL;
    printf("ptfname=%s,NpicSN=%d\n",ptfname,NpicSN);
  }
}
void initPoint(int picID) {
  char coor[6];
  int totp=0;
  int x,y;
  char *split;
  ptClick.x = -1;
  ptClick.y = -1;
  sprintf(ptfname,"%s/point%d.txt",fDirectory,picSN[picID]);  
  if (access(ptfname, F_OK) == 0) {
    // file exists
    ptFile = fopen(ptfname,"rt");

    // Read line-by-line until fgets returns NULL
    while (fgets(buffer, sizeof(buffer), ptFile) != NULL) {
        split=strstr(buffer,",");
        if(split!=NULL && split!=buffer) {
          memcpy(coor,buffer,split-buffer);coor[split-buffer]=0;
          x=atoi(coor);
          strcpy(coor,split+1);
          y=atoi(coor);
          pt[picID][totp].x=x; pt[picID][totp].y=y;
          printf("%s,(picID=%d),[%d],x=%4d,y=%4d\n", ptfname,picID,totp,pt[picID][totp].x, pt[picID][totp].y);
          totp++;
          if(totp>15) break;
        } 
    }
    if(ptFile!=NULL) fclose(ptFile); ptFile=NULL;
    nPt[picID] = totp;
  } else {
    // file doesn't exist
    ptFile = fopen(ptfname,"wt");
    if(ptFile!=NULL) fclose(ptFile); ptFile=NULL;
    nPt[picID] = 0;
  }
}
int getPoint(int picID,int x,int y) {
  int n;
  //search
  for(n=0;n<nPt[picID];n++) {
    if(x==pt[picID][n].x && y==pt[picID][n].y) {
      return n;
    }
  }
  return -1;
}
int putPoint(int picID,int x,int y) {
  int n=nPt[picID],i;
  char buffer[MAX_LINE_LENGTH];
  char ptfname[256];
  if(getPoint(picID,x,y)==-1) 
  {
    pt[picID][n].x=x;pt[picID][n].y=y;
    sprintf(ptfname,"%s/point%d.txt",fDirectory,picSN[picID]);  
    n++;if(n>15) n=15; nPt[picID]=n; 
    ptFile = fopen(ptfname,"wt");
    for(i=0;i<nPt[picID];i++) {
        sprintf(buffer,"%d,%d\n", pt[picID][i].x, pt[picID][i].y);
        fputs(buffer, ptFile);
    }
    fclose(ptFile);

    return n;
  }
  return n;
}
int nowpicID=1;
void DrawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius) {
    const int32_t diameter = (radius * 2);
    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y) {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0) {
            ++y;
            error += ty;
            ty += 2;
        }
        if (error > 0) {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

// Configure button specs: X, Y, Width, Height
Button myButton[6]= { { {  40, 50, 40, 40 }, false, "" },
                      { {  90, 50, 40, 40 }, false, "" },
                      { { 140, 50, 40, 40 }, false, "" },
                      { { 190, 50, 40, 40 }, false, "" },
                      { { 240, 50, 40, 40 }, false, "" },
                      { { 290, 50, 40, 40 }, false, "" },
                    };
const int width  = 2160;
const int height = 3840;
// Set up source rectangle (e.g., cropping a 400x300 chunk starting at x=50, y=50)
//SDL_Rect srcRect = {1200, 1000, 1056, 594};
SDL_Rect srcRect   = {0, 0, 972, 576};
// Set up destination rectangle (same size for true 1:1 "real" size)
SDL_Rect srcMRect  = {0, 0, 972, 576};
SDL_Rect destRect  = {0, 0, 972, 576}; 
SDL_Rect destMRect = {0, 0, 972+108, 576+64}; 

void Draw4K(SDL_Surface* surface,SDL_Renderer* renderer0, int yid) {
    SDL_Rect img_rect2;
    img_rect2.x    = 0;
    img_rect2.y    = 0;
    img_rect2.w    = width;
    img_rect2.h    = height;
    // Create a hidden window & 4K software renderer
    SDL_Surface* surface3 = IMG_Load(myButton[yid].fstr);
//  SDL_Window* window2 = SDL_CreateWindow("4K Circle", 0, 0, width, height, SDL_WINDOW_HIDDEN);
    SDL_Window* window2 = SDL_CreateWindow("4K Circle", 0, 0, width, height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer2 = SDL_CreateRenderer(window2, -1, SDL_RENDERER_SOFTWARE);
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer2, surface3);
    SDL_Surface* surface2 = SDL_ConvertSurfaceFormat(surface3, SDL_PIXELFORMAT_ARGB8888, 0);
    
    // Set background to black
//  SDL_SetRenderDrawColor(renderer2, 255, 255, 255, 255);
//  SDL_RenderClear(renderer2);

//  SDL_RenderCopy(renderer2, texture, &img_rect2, &img_rect2);
//  SDL_RenderPresent(renderer2);

    // Draw a white circle in the center with a radius of 500 pixels
    printf("%d\n",__LINE__);
//    SDL_SetRenderDrawColor(renderer2, 255, 255, 255, 255);
//    SDL_RenderClear(renderer2);
    SDL_RenderCopy(renderer2, texture2, NULL, &img_rect2); 
//  SDL_RenderPresent(renderer2);    //present renderer
//    SDL_SetRenderDrawColor(renderer2, 0,  0, 0, 255);
//    DrawCircle(renderer2, 100, 100, 100); 

        int n=nPt[0];
        if(n>10) n=10;
        for(int i=0;i<n;i++) {
//        circleColor(renderer, pt[nowpicID][i].x, pt[nowpicID][i].y, 50, 0xFF0000FF);
          SDL_SetRenderDrawColor(renderer2, 0, 0, 255, 255); 
          DrawCircle(renderer2, pt[nowpicID][i].x-srcRect.x, pt[nowpicID][i].y-srcRect.y, 400);
        }
  
    SDL_RenderPresent(renderer2);    //present renderer
    SDL_Delay(500);
//  SDL_RenderPresent(renderer2);
    SDL_RenderReadPixels(renderer2, NULL,surface2->format->format, surface2->pixels, surface2->pitch);
    SDL_RenderClear(renderer2); 

    // Allocate buffer for 4K pixels
//  std::vector<Uint32> pixels(width * height);
//  SDL_LockSurface(surface);
    // Calculate the total size in bytes (pitch * height)
    // Assuming 32-bit (4 bytes) pixels, we divide by 4 to get the vector size
    size_t pixelCount = (surface2->w * surface2->h) ;
    printf("%d,%d,%d,%d,%d\n",__LINE__,pixelCount,surface2->pitch,surface2->h,surface2->w);  
    // Create the vector and directly copy the pixel memory
    Uint32* pixelsData = static_cast<Uint32*>(surface2->pixels);
    std::vector<Uint32> pixels2(pixelsData, pixelsData + pixelCount);
//  std::vector<Uint32> pixels(pixels2);
    // Unlock surface when done
//  SDL_RenderReadPixels(renderer2, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);

    // Read the pixels from the renderer into memory
//  SDL_RenderReadPixels(renderer2, NULL, SDL_PIXELFORMAT_ARGB8888, pixels.data(), width * sizeof(Uint32));

    // Save as 4K JPG (quality: 90)
//  stbi_write_jpg("output_4k.jpg", width, height, 4, pixels.data(), 90);
    sprintf(ptfname,"%s/y%04d.jpg",fDirectory,yid);  
//  stbi_write_jpg(ptfname, width, height, 4, pixels.data(), 90);
    stbi_write_jpg(ptfname, width, height, 4, pixels2.data(), 90);
//  SDL_UnlockSurface(surface);

    // Clean up
    SDL_DestroyRenderer(renderer2);
    SDL_DestroyWindow(window2);
}
int sdl_main(int argc, char* argv[]) {
    int i;
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Init Failed: %s\n", SDL_GetError());
        return 1;
    }

    initfDirectory();
    printf("Get directory [%s]<======\n",fDirectory);
    
    initpicSN();
    if(NpicSN<3) {
      printf("No pictures are assigned.\n");exit(0);
    } 
    for(i=0;i<NpicSN;i++) {
      printf("picSN[%4d]=%04d\n",i,picSN[i]);
    }

    initPoint(0);
//  putPoint(0,7,7);
    initPoint(1);
    initPoint(2);

    // Initialize SDL_image for JPG
    int flags = IMG_INIT_JPG;
    if ((IMG_Init(flags) & flags) != flags) {
        printf("IMG Init Failed: %s\n", IMG_GetError());
        SDL_Quit();
    }

    // Create Window and Renderer
    SDL_Window* window = SDL_CreateWindow("Real Size JPG", SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 972, 576, SDL_WINDOW_SHOWN);
//  SDL_Window* window = SDL_CreateWindow("Real Size JPG", 0, 0, width, height, SDL_WINDOW_SHOWN);
//  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    // Load JPG into a surface
    sprintf(myButton[0].fstr,"%s/x%04d.jpg",fDirectory,picSN[0]);
    sprintf(myButton[1].fstr,"%s/x%04d.jpg",fDirectory,picSN[1]);
    sprintf(myButton[2].fstr,"%s/x%04d.jpg",fDirectory,picSN[2]);
    SDL_Surface* surface = IMG_Load(myButton[0].fstr);
    if (!surface) {
        printf("Failed to load image: %s\n", IMG_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Convert surface to texture
    SDL_Rect img_rect;
#if 1
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
#else
    SDL_Texture* texture = SDL_CreateTexture(renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        surface->w, surface->h);

    SDL_LockTexture(texture, &img_rect, &surface->pixels, &surface->pitch);
    // paint into surface pixels
    SDL_UnlockTexture(texture);
#endif
    SDL_QueryTexture(texture, NULL, NULL, &img_rect.w, &img_rect.h);
    printf("(%3d) w=%4d,h=%4d\n",__LINE__,img_rect.w, img_rect.h);
//  SDL_FreeSurface(surface); // We don't need the surface anymore
     
    if (TTF_Init() < 0)
    {
      printf("Couldn't initialize SDL TTF: %s\n", SDL_GetError());
      exit(1);
    }
    //this opens a font style and sets a size
//  TTF_Font* Sans = TTF_OpenFont("FreeSans.ttf", 24);
    TTF_Font* Sans = TTF_OpenFont("m.ttf", 25);
//  TTF_Font* Sans = TTF_OpenFont( "C:/Windows/Fonts/kaiu.ttf", 25 );//???, ????C:/Windows/Fonts????

    // this is the color in rgb format,
    // maxing out all would give you the color white,
    // and it will be your text's color
    SDL_Color White = {255, 255, 255, 255};

 //???????????

    wchar_t MyString[]=L"arithai.com ????? v1.1!";
    //????????MyString????????????
    //???????
    int MyStringLengh = wcslen(MyString); //???????????, ????????
    char DecToHex[20]; //????????, ??????????
    Uint16 PrintMyString[MyStringLengh]; //???????????????, ?????????????
    //?????????
    for (int i = 0; i < MyStringLengh; i++){
      //itoa(MyString[i], DecToHex, 16);  
        snprintf(DecToHex, sizeof(DecToHex), "%02X", MyString[i]); 
 //???MyString?????????????????, ????????DecToHex
        PrintMyString[i]= strtol(DecToHex,NULL,16);//???DecToHex?????????????, ??????PrintMyString
    }
    PrintMyString[MyStringLengh]={0};//????????????, ?????????????????
 
    // as TTF_RenderText_Solid could only be used on
    // SDL_Surface then you have to create the surface first
    SDL_Surface* surfaceMessage =
//    TTF_RenderText_Solid(Sans, "arithai.com v1.1!", White); 
//    TTF_RenderText_Blended(Sans, "arithai.com ????? v1.1!", White);
      TTF_RenderUNICODE_Blended(Sans, PrintMyString, White);
//    TTF_RenderUNICODE_Solid( Sans, PrintMyString, White );
    // now you can convert it into a texture
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect Message_rect; //create a rect
    Message_rect.x = 972-400;  //controls the rect's x coordinate 
    Message_rect.y = 576-100; // controls the rect's y coordinte
//  Message_rect.w = 200; // controls the width of the rect
//  Message_rect.h = 200; // controls the height of the rect
    Message_rect.w = surfaceMessage->w; // controls the width of the rect
    Message_rect.h = surfaceMessage->h; // controls the height of the rect

// (0,0) is on the top left of the window/screen,
// think a rect as the text's box,
// that way it would be very simple to understand

// Now since it's a texture, you have to put RenderCopy
// in your game loop area, the area where the whole code executes

// you put the renderer's name first, the Message,
// the crop size (you can ignore this if you don't want
// to dabble with cropping), and the rect which is the size
// and coordinate of your texture

    int zoomFactorX = 18; // Pixels to scale per scroll
    int zoomFactorY = 32; // Pixels to scale per scroll
    int offset_x = 0, offset_y = 0;
    bool is_dragging = false;
    bool is_zooming = false;
    bool is_ClickingOnly = false;
    bool is_leftclick = false;
    // Rendering loop 
    int quit = 0;
    int is_extending = 0;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            else if (e.type == SDL_MOUSEWHEEL) {
                is_dragging = false;
                is_zooming = true;
                is_extending = 0;
              //SDL_QueryTexture(texture, NULL, NULL, &img_rect.w, &img_rect.h);
                // Adjust dimensions on mouse wheel
                if (e.wheel.y > 0) { // Scroll Up (Zoom In)
                    destRect.w += zoomFactorX;
                    destRect.h += zoomFactorY;
                    destRect.x -= zoomFactorX / 2; // Center the zoom
                    destRect.y -= zoomFactorY / 2;
                } else if (e.wheel.y < 0) { // Scroll Down (Zoom Out)
                    destRect.w -= zoomFactorX;
                    destRect.h -= zoomFactorY;
                    destRect.x += zoomFactorX / 2;
                    destRect.y += zoomFactorY / 2;
                }
             // srcRect.x = img_rect.x;
             // srcRect.y = img_rect.y;
//Remapping if zoom out 2026.7.4
                if(srcRect.x>0 && srcRect.y>0 && destRect.x>0 && destRect.y>0 && destRect.h>48) {
                  printf("**[%4d,%4d,%4d,%4d],z=(%4d,%4d,%4d),s=(%4d,%4d,%4d,%4d),d=(%4d,%4d,%4d,%4d)\n",
                       img_rect.x, img_rect.y, img_rect.w, img_rect.h,  
                       zoomFactorX,zoomFactorY,e.wheel.y,
                       srcRect.x, srcRect.y, srcRect.w, srcRect.h,
                       destRect.x, destRect.y, destRect.w, destRect.h
                       );
                  srcMRect.x=srcRect.x-srcRect.x*destRect.x/972;
                  srcMRect.y=srcRect.y-srcRect.y*destRect.y/576;
                  srcMRect.w=972*srcRect.w/destRect.w;
                  srcMRect.h=576*srcRect.h/destRect.h;
                  if(srcMRect.h>3840) {
                    srcMRect.x=0;
                    srcMRect.y=0;
                    srcMRect.w=2160;
                    srcMRect.h=3840;

                    destMRect.x=324;
                    destMRect.y=0;
                    destMRect.w=324;
                    destMRect.h=576;               
                  }  
                  else {
                    destMRect.x=0;
                    destMRect.y=0;
                    destMRect.w=972;
                    destMRect.h=576;               
                  }
                  is_extending = 1;
                }
                SDL_QueryTexture(texture, NULL, NULL, &img_rect.w, &img_rect.h);

                printf(">>[%4d,%4d,%4d,%4d],z=(%4d,%4d,%4d),s=(%4d,%4d,%4d,%4d),d=(%4d,%4d,%4d,%4d)\n",
                       img_rect.x, img_rect.y, img_rect.w, img_rect.h,  
                       zoomFactorX, zoomFactorY,e.wheel.y,
                       srcMRect.x, srcMRect.y, srcMRect.w, srcMRect.h,
                       destRect.x, destRect.y, destRect.w, destRect.h
                       );
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouse_x, mouse_y;
                is_zooming = false;
                is_ClickingOnly = false;
                Uint32 mouseState = SDL_GetMouseState(&mouse_x, &mouse_y);
                if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                  is_leftclick = true;
                }
                //right click cancel it
                if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
                  is_leftclick = false;
                }
                printf("mx=%4d,my=%4d,ix=%4d,iy=%4d,iw=%4d,ih=%4d\n",
                      mouse_x, mouse_y,
                      img_rect.x,img_rect.y,img_rect.w,img_rect.h);
                if (isMouseOver(mouse_x, mouse_y, myButton[4].rect)) {
                }
                else if (isMouseOver(mouse_x, mouse_y, myButton[5].rect)) {
                }
                else if (isMouseOver(mouse_x, mouse_y, myButton[0].rect)) {
                      myButton[0].isPressed = true;
                      printf("Button Clicked!\n");
                      surface = IMG_Load(myButton[0].fstr);
                      texture = SDL_CreateTextureFromSurface(renderer, surface);
                      nowpicID=0;
                }
                else if (isMouseOver(mouse_x, mouse_y, myButton[1].rect)) {
                      myButton[1].isPressed = true;
                      printf("Button Clicked!\n");
                      surface = IMG_Load(myButton[1].fstr);
                      texture = SDL_CreateTextureFromSurface(renderer, surface);
                      nowpicID=1;
                }
                else if (isMouseOver(mouse_x, mouse_y, myButton[2].rect)) {
                      myButton[2].isPressed = true;
                      printf("Button Clicked!\n");
                      surface = IMG_Load(myButton[2].fstr);
                      texture = SDL_CreateTextureFromSurface(renderer, surface);                     
                      nowpicID=2;
                }
                else if (isMouseOver(mouse_x, mouse_y, myButton[3].rect)) {
                      myButton[3].isPressed = true;
                      Draw4K(surface,renderer,0);
                      Draw4K(surface,renderer,1);
                      Draw4K(surface,renderer,2);
                      nowpicID=-1;
                }
                else {
                  if (mouse_x >= img_rect.x && mouse_x <= img_rect.x + img_rect.w &&
                    mouse_y >= img_rect.y && mouse_y <= img_rect.y + img_rect.h) {
                    is_dragging = true;
                    offset_x = mouse_x - img_rect.x;
                    offset_y = mouse_y - img_rect.y;
                    printf("mx=%4d,my=%4d,ix=%4d,iy=%4d,iw=%4d,ih=%4d\n",
                        mouse_x, mouse_y,
                        img_rect.x,img_rect.y,img_rect.w,img_rect.h);

                  }
                  is_ClickingOnly = true;
                }
              
            } else if (e.type == SDL_MOUSEBUTTONUP) {
                if(is_ClickingOnly) {
                  int mouse_x, mouse_y;
                  int x,y;
                  SDL_GetMouseState(&mouse_x, &mouse_y);
                  x = mouse_x - img_rect.x;   
                  y = mouse_y - img_rect.y;
                  if(ptClick.x==-1 && ptClick.y==-1) {
                    ptClick.x = x;
                    ptClick.y = y;
                  }
                  else {
                    x = x-ptClick.x;
                    y = y-ptClick.y;
                    if(is_leftclick) {
                      // Left click action here
                      if(sqrt(x*x+y*y)<50) {
                        printf("(%3d) x=%3d,y=%3d,px=%3d,py=%3d,now=%3d,pic=%3d\n",__LINE__,x,y,
                          ptClick.x,ptClick.y,nowpicID,picSN[nowpicID]); 
                        putPoint(nowpicID, ptClick.x, ptClick.y);
                        printf("Click,%3d,%4d,%4d,%4d,%4d\n",nowpicID, mouse_x, mouse_y,
                           img_rect.x,img_rect.y,img_rect.w,img_rect.h);
                      }
                    }
                    ptClick.x=-1;
                    ptClick.y=-1;
                  }
                } 
                is_dragging = false;
                if(  img_rect.x<=0 && (img_rect.x+img_rect.w)>=0
                   &&img_rect.y<=0 && (img_rect.y+img_rect.h)>=0 ) {
                   srcRect.x = -img_rect.x;
                   srcRect.y = -img_rect.y;
                //   srcRect.w = img_rect.w;
                //   srcRect.h = img_rect.h;
                   zoomFactorX = 18;
                   zoomFactorY = 32;
                   printf("sx=%4d,sy=%4d,ix=%4d,iy=%4d,iw=%4d,ih=%4d\n",
                      srcRect.x, srcRect.y,
                      img_rect.x,img_rect.y,img_rect.w,img_rect.h);
                  if (e.button.button == SDL_BUTTON_LEFT) {
                      myButton[0].isPressed = false; // Reset state when released
                      myButton[1].isPressed = false; // Reset state when released
                      myButton[2].isPressed = false; // Reset state when released
                  }
                }
            } else if (e.type == SDL_MOUSEMOTION) {
                if (is_dragging) {
                    int mouse_x, mouse_y;
                    SDL_GetMouseState(&mouse_x, &mouse_y);
                    img_rect.x = mouse_x - offset_x;
                    img_rect.y = mouse_y - offset_y;

                    printf("mx=%4d,my=%4d,ix=%4d,iy=%4d,iw=%4d,ih=%4d\n",
                        mouse_x, mouse_y,
                        img_rect.x,img_rect.y,img_rect.w,img_rect.h);


                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the cropped portion
        if(is_zooming) 
        {
          if(is_extending==1) {
  //       printf("is_extending..(%4d,%4d,%4d,%4d)\n",destMRect.x,destMRect.y,destMRect.w,destMRect.h);
           SDL_RenderCopy(renderer, texture, &srcMRect, &destMRect);            
          }
          else {
            SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
          }
        }
        else { 
          SDL_RenderCopy(renderer, texture, NULL, &img_rect);
        }      
        if (myButton[0].isPressed) {
            SDL_SetRenderDrawColor(renderer, 46, 204, 113, 255); // Green click accent
        } else {
            SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255); // Blue normal mode
        }
        SDL_RenderFillRect(renderer, &myButton[0].rect);
        if (myButton[1].isPressed) {
            SDL_SetRenderDrawColor(renderer, 46, 204, 113, 255); // Green click accent
        } else {
            SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255); // Blue normal mode
        }
        SDL_RenderFillRect(renderer, &myButton[1].rect);
        if (myButton[2].isPressed) {
            SDL_SetRenderDrawColor(renderer, 46, 204, 113, 255); // Green click accent
        } else {
            SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255); // Blue normal mode
        }
        SDL_RenderFillRect(renderer, &myButton[2].rect);
        if (myButton[3].isPressed) {
            SDL_SetRenderDrawColor(renderer, 46, 204, 113, 255); // Green click accent
        } else {
            SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255); // Blue normal mode
        }
        SDL_RenderFillRect(renderer, &myButton[3].rect);

        SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255); // Blue normal mode
        SDL_RenderFillRect(renderer, &myButton[4].rect);
        SDL_RenderFillRect(renderer, &myButton[5].rect);
//DrawCircle
        int n=nPt[nowpicID];
        if(n>10) n=10;
        for(int i=0;i<n;i++) {
//        circleColor(renderer, pt[nowpicID][i].x, pt[nowpicID][i].y, 50, 0xFF0000FF);
          SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
          DrawCircle(renderer, pt[nowpicID][i].x-srcRect.x, pt[nowpicID][i].y-srcRect.y, 50);
        }
        if(ptClick.x>=0 && ptClick.y>=0) {
          SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); 
          DrawCircle(renderer, ptClick.x-srcRect.x, ptClick.y-srcRect.y, 50);
        } 


        SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();

    return 0;
}
