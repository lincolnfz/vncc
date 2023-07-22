#include "eAVRender.h"
#include "base/misctool.h"
#include <json.hpp>
#include "eControlTrans.h"
//#include <stdio.h>
//#include <stdlib.h>
#include "../common/defThread.h"


#define _XOPEN_SOURCE 600 /* for usleep */
#include <stdio.h>
#include <stdlib.h>

#include <qlogging.h>
#include <dlfcn.h>
#include <libyjrtc.h>

//#ifdef __cplusplus
extern "C"
{
//#endif

#include <libavutil/avstring.h>
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
#include <libavutil/mathematics.h>
#include <libavutil/time.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/imgutils.h>
#include <SDL2/SDL.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/opt.h>
#include <libavutil/avutil.h>
#include <libavutil/error.h>

//#ifdef __cplusplus
};
//#endif
/*
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avdevice.lib")
#pragma comment(lib, "avfilter.lib")
#pragma comment(lib, "swresample.lib")
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "sdl2.lib")
#pragma comment(lib, "fftw3.lib")
*/

#define AUDIO_DATA_TYPE AV_SAMPLE_FMT_S16
#define AUDIO_PLAY_SAMPLE 16000
#define AUDIO_SAMPLE 48000
#define AUDIO_BUF_SIZE 384000
#define min(a,b) ( (a) < (b) )? (a):(b)
#define max(a,b) ( (a) > (b) )? (a):(b)

/*----------------------??????-----------------*/
/*????*/
#define EXIT_NETWORK 1

/*????*/
#define EXIT_MULTI 222

/*????*/
#define EXIT_OCCUPIED 3

/*??????*/
#define EXIT_BACKUP_RESTORE 4


extern bool gDone;
extern eControlTrans* gTrans;
extern int g_Bitrate;
float g_angle = 0.f;
extern bool g_bAutorate;
uint64_t g_laucnch_TS;
int g_fps;
int g_width;
int g_height;
int g_order_id;
int g_hwdeoce_type;
bool b_h265 = true;
extern bool g_multi_win_platform;
extern bool g_play_logo_animation;
bool bStopRender = false;
uint64_t g_remote_render_ts = 0;
bool g_b_check_run = false;
eVideoRender::PIXEL_FORMAT out_fmt;
int out_idx = 0;
int out_frame_w = 0;
int out_frame_h = 0;

/******************************************************************************
 *函数名   :ring_buf_init
 *函数功能 :构造一个空环形缓冲区
 *输入参数 :r 环形缓冲区控制块
 *返回值   :0表示成功
 *****************************************************************************/
int ring_buf_create(ring_buf_t* r, unsigned char* buf, unsigned int len)
{
	r->buf = buf;
	r->size = len;
	r->front = r->rear = 0;
	return buf == nullptr;
}

/******************************************************************************
 *函数名   :ring_buf_realloc
 *函数功能 :重新构造一个空环形缓冲区
 *输入参数 :r 环形缓冲区控制块
 *返回值   :0表示成功
 *****************************************************************************/
int ring_buf_realloc(ring_buf_t* old_r, ring_buf_t* new_r, unsigned char* new_buf, unsigned int new_size) {
	if (old_r->size >= new_size) {
		//新的ringbuf比原来的小,就不创建
		return -1;
	}
	int ret = ring_buf_create(new_r, new_buf, new_size);
	if (!ret) {
		while (true) {
			unsigned char *szTmp = (unsigned char*)malloc(new_size);
			unsigned int len = ring_buf_get(old_r, szTmp, new_size);
			if (len > 0) {
				ring_buf_put(new_r, szTmp, len);
				free(szTmp);
			}
			else {
				free(szTmp);
				break;
			}
		}
	}


	return ret;
}

/**********************************************************************
 *函数名   :ring_buf_clr
 *函数功能 :清空环形缓冲区
 *输入参数 :r - 待清空的环形缓冲区
 *返回值   :None
 *********************************************************************/
void ring_buf_clr(ring_buf_t* r)
{
	r->front = r->rear = 0;
}

/**********************************************************************
 *函数名   :ring_buf_len
 *函数功能 :计算环形缓冲区有效容量 (字节为单位)
 *输入参数 :r.环形缓冲区控制块
 *返回值   :环形缓冲区中有效字节数
 *********************************************************************/
unsigned int ring_buf_len(ring_buf_t* r)
{
	return r->rear - r->front;
}

/**********************************************************************
 *函数名   :ring_buf_len
 *函数功能 :计算环形缓冲区剩余容量 (字节为单位)
 *输入参数 :r.环形缓冲区控制块
 *返回值   :环形缓冲区中剩余字节数
 *********************************************************************/
unsigned int ring_buf_remain(ring_buf_t* r) {
	return r->size - ring_buf_len(r);
}

/**********************************************************************
 *函数名   :ring_buf_put
 *函数功能 :将指定长度的数据放到环形缓冲区中
 *输入参数 :buf - 数据缓冲区
 *          len - 缓冲区长度
 *返回值   :实际放到中的数据
 *********************************************************************/
unsigned int ring_buf_put(ring_buf_t* r, unsigned char* buf, unsigned int len)
{
	unsigned int i;
	unsigned int space;
	space = r->size + r->front - r->rear;
	len = min(len, space);                  /*得到实际写入的数据长度*/
	/*min(空闲空间大小,从real_in开始到缓冲区结尾的空间) -------------*/
	i = min(len, r->size - r->rear % r->size);
	/*
	 * 当len > l时，拷贝buffer中剩余的内容
	 *而剩余的大小为len - l
	 */
	memcpy(r->buf + r->rear % r->size, buf, i);
	memcpy(r->buf, buf + i, len - i);
	r->rear += len;
	return len;

}


/**********************************************************************
 *函数名   :rueueGet
 *函数功能 :从环形缓冲区中读取指定长度的数据
 *输入参数 :len - 读取长度
 *输出参数 :buf - 输出数据缓冲区
 *返回值   :实际读取长度
 *********************************************************************/
unsigned int ring_buf_get(ring_buf_t* r, unsigned char* buf, unsigned int len)
{
	unsigned int i;
	unsigned int space;
	space = r->rear - r->front;
	len = min(len, space);
	i = min(len, r->size - r->front % r->size);
	memcpy(buf, r->buf + r->front % r->size, i);
	memcpy(buf + i, r->buf, len - i);
	r->front += len;
	return len;
}

std::string GenCollectStr(const std::string msg) {
	nlohmann::json root;
	if (msg.length() > 1024) {
		root["msg"] = msg.substr(0, 1020);
	}
	else {
		root["msg"] = msg;
	}
	std::string out = root.dump();
	return out;
}

typedef void (*FnYJNetInit)(datacallback v_p, datacallback a_p, callback m_p);
typedef void (*FnYJNetConnect)(char* str_param_json);
typedef void (*FnYJNetDisConnect)();
typedef void (*FnYJNetVideoControl)(char* str_param_json);

FnYJNetInit pYJNetInit = nullptr;
FnYJNetConnect pYJNetConnect = nullptr;
FnYJNetDisConnect pYJNetDisConnect = nullptr;
FnYJNetVideoControl pYJNetVideoControl = nullptr;

//-----------------------------------//
typedef struct DecoderContext {
	int color_format;
	struct AVCodec* codec;
	struct AVCodecContext* codec_ctx;
	struct AVFrame* src_frame;
	struct AVFrame* dst_frame;
	struct SwsContext* convert_ctx;
	struct SwrContext* convert_audio_ctx;
	int frame_ready;
} DecoderContext;

DecoderContext *g_video_dec_ctx, *g_audio_dec_ctx;

eAVRender* eAVRender::g_inst = nullptr;
static unsigned char tmpbuf[19120128];

char szorderid[64];
AVPixelFormat hw_pix_fmt = AV_PIX_FMT_NONE;
AVBufferRef* hw_buf_ref = nullptr;
AVFrame* sw_frame = nullptr;
enum AVHWDeviceType hw_device_type = AV_HWDEVICE_TYPE_NONE;
AVFrame* pYUV420Frame = nullptr;
SwsContext* sws_context = nullptr;
bool bHWacc = true; //是否使用硬件加速
std::vector<unsigned char> yuv_vec;
unsigned char a_v_render_state = 0;
#define VIDEO_RENDER_STATE_OK 0x2
#define AUDIO_RENDER_STATE_OK 0x1
bool b_play_done = false;
bool b_ratio_send = false;

static enum AVPixelFormat get_hw_format(AVCodecContext* ctx,
	const enum AVPixelFormat* pix_fmts)
{
	const enum AVPixelFormat* p;

	for (p = pix_fmts; *p != -1; p++) {
		if (*p == hw_pix_fmt)
			return *p;
	}

	fprintf(stderr, "Failed to get HW surface format.\n");
	return AV_PIX_FMT_NONE;
}

static int hw_decoder_init(AVCodecContext* ctx, const enum AVHWDeviceType type)
{
	int err = 0;
	//创建硬件加速器实例
	if ((err = av_hwdevice_ctx_create(&hw_buf_ref, type,
		NULL, NULL, 0)) < 0) {
		printf("Failed to create specified HW device. code:%d\n", err);
		return err;
	}
	//告诉解码器，使用我们创建的硬件加速器
	ctx->hw_device_ctx = av_buffer_ref(hw_buf_ref);
	printf("hw_decoder_init ok\n");
	return err;
}

int callocAVFrame(AVFrame** frame, int width, int height) {
	int ret = -1;
	if (*frame && ((*frame)->width != width || (*frame)->height != height)) {
		av_frame_free(frame);
		frame = nullptr;
	}
	if (*frame == nullptr) {
		*frame = av_frame_alloc();
		(*frame)->format = AV_PIX_FMT_YUV420P;
		(*frame)->width = width;
		(*frame)->height = height;
		ret = av_frame_get_buffer(*frame, 0);
	}
	else {
		ret = 0;
	}
	return ret;
}

void InitH264Decode()
{
    g_laucnch_TS = getNowTickMs();
	uint64_t ts = 0;
    ts = getNowTickMs() - g_laucnch_TS;
	printf("###---InitH264Decode begin: %lld(ms)\n", ts);
	std::vector<AVHWDeviceType> avhw;// = { AV_HWDEVICE_TYPE_D3D11VA };
	{
		void* i = 0;
		const AVCodec* p;

		//while ((p = av_codec_iterate(&i))) {
			//printf("name: %s\n", p->name);
		//}

		while ((hw_device_type = av_hwdevice_iterate_types(hw_device_type)) != AV_HWDEVICE_TYPE_NONE) {
			printf("acc name: %s\n", av_hwdevice_get_type_name(hw_device_type));
            //printLogMsg("render", "acc name: %s", av_hwdevice_get_type_name(hw_device_type));
			avhw.push_back(hw_device_type);
		}
	}
	//av_register_all();
	g_video_dec_ctx = (DecoderContext*)calloc(1, sizeof(DecoderContext));
	g_video_dec_ctx->color_format = AV_PIX_FMT_YUV420P;

	g_video_dec_ctx->codec = const_cast<AVCodec*>(avcodec_find_decoder(b_h265 ? AV_CODEC_ID_H265 : AV_CODEC_ID_H264));

	printf("##use bHWacc %d\n", bHWacc);
	if (bHWacc && g_hwdeoce_type == 1) {
		for (auto it : avhw) {
			g_video_dec_ctx->codec_ctx = avcodec_alloc_context3(g_video_dec_ctx->codec);
			g_video_dec_ctx->codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
			//g_video_dec_ctx->codec_ctx->flags2 |= AV_CODEC_FLAG2_CHUNKS;
			g_video_dec_ctx->codec_ctx->flags |= AV_CODEC_FLAG_LOW_DELAY;
			g_video_dec_ctx->codec_ctx->flags |= AV_CODEC_FLAG2_LOCAL_HEADER;
			g_video_dec_ctx->codec_ctx->get_format = get_hw_format;
			if (hw_decoder_init(g_video_dec_ctx->codec_ctx, it) < 0) {
				avcodec_free_context(&(g_video_dec_ctx->codec_ctx));
				g_video_dec_ctx->codec_ctx = nullptr;
				//printf("hw_decoder_init fail\n");
				printf("##hw_decoder_init fail\n");
			}
			else {
				hw_device_type = it;
				break;
			}
		}
	}
	if (hw_device_type == AV_HWDEVICE_TYPE_NONE) {
		g_video_dec_ctx->codec_ctx = avcodec_alloc_context3(g_video_dec_ctx->codec);
		g_video_dec_ctx->codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
		//g_video_dec_ctx->codec_ctx->flags2 |= AV_CODEC_FLAG2_CHUNKS;
		g_video_dec_ctx->codec_ctx->flags |= AV_CODEC_FLAG_LOW_DELAY;
		g_video_dec_ctx->codec_ctx->flags |= AV_CODEC_FLAG2_LOCAL_HEADER;
		printf("cpu render\n");
        //printLogMsg("render", "cpu render");
		gTrans->SendCollectData(std::string("connect_media_log"), GenCollectStr("cpu render"), true);
	}
	else {
		for (int i = 0;; ++i) {
			const AVCodecHWConfig* config = avcodec_get_hw_config(g_video_dec_ctx->codec, i);
			if (!config) {
				break;
			}
			if ((config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX) && hw_device_type == config->device_type) {
				hw_pix_fmt = config->pix_fmt;
				break;
			}
		}
		char szmsg[256] = { 0 };
        sprintf(szmsg, "select acc name: %s pixfmt: %d\n", av_hwdevice_get_type_name(hw_device_type), hw_pix_fmt);
		printf(szmsg);
        //printLogMsg("render", "selet acc name: %s pixfmt: %d", av_hwdevice_get_type_name(hw_device_type), hw_pix_fmt);
        //gTrans->SendCollectData(std::string("connect_media_log"), GenCollectStr(szmsg), true);
	}
	

	if (avcodec_open2(g_video_dec_ctx->codec_ctx, g_video_dec_ctx->codec, NULL) < 0)
	{
		printf("avcodec open error\n");
		return;
	}

	g_video_dec_ctx->src_frame = av_frame_alloc();
	g_video_dec_ctx->dst_frame = av_frame_alloc();
	sw_frame = av_frame_alloc();
    ts = getNowTickMs() - g_laucnch_TS;
	printf("###---InitH264Decode ok: %lld(ms)\n", ts);
	return;
}

static void video_decode(AVCodecContext* dec_ctx, AVFrame* frame, AVPacket* pkt);
static void audio_decode(AVCodecContext* dec_ctx, AVFrame* frame, AVPacket* pkt);

void DecodeH264(char* input_buf, int m_num_bytes, int pkt_pts)
{
	/*AVPacket packet;
	packet.data = (uint8_t*)input_buf,
		packet.size = m_num_bytes,
		packet.pts = pkt_pts;*/
	AVPacket* pkt = av_packet_alloc();
	pkt->data = (uint8_t*)input_buf;
	pkt->size = m_num_bytes;
	pkt->pts = pkt_pts;
	int frameFinished = 0;
	//printf("avcodec_decode_video2...\n");
	video_decode(g_video_dec_ctx->codec_ctx, g_video_dec_ctx->src_frame, pkt);
	int res = 0; //avcodec_decode_video2(g_video_dec_ctx->codec_ctx, g_video_dec_ctx->src_frame, &frameFinished, &packet);
	av_packet_free(&pkt);
	return;
}

void DecodeOpus(char* input_buf, int m_num_bytes, int pkt_pts)
{
	/*AVPacket packet;
	packet.data = (uint8_t*)input_buf,
		packet.size = m_num_bytes,
		packet.pts = pkt_pts;*/
	AVPacket* pkt = av_packet_alloc();
	pkt->data = (uint8_t*)input_buf;
	pkt->size = m_num_bytes;
	pkt->pts = pkt_pts;
	int frameFinished = 0;
	//printf("avcodec_decode_video2...\n");
	audio_decode(g_audio_dec_ctx->codec_ctx, g_audio_dec_ctx->src_frame, pkt);
	int res = 0; //avcodec_decode_video2(g_video_dec_ctx->codec_ctx, g_video_dec_ctx->src_frame, &frameFinished, &packet);
	av_packet_free(&pkt);
	return;
}

static void pgm_save(unsigned char* buf, int wrap, int xsize, int ysize,
	char* filename)
{
	FILE* f;
	int i;
	static bool head = true;
	f = fopen(filename, "ab");
	//if(head)
	//	fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);
	for (i = 0; i < ysize; i++)
		fwrite(buf + i * wrap, 1, xsize, f);
	fclose(f);
}

static void raw_save(unsigned char* buf, int len, const char* filename) {
	FILE* f;
	int i;
	f = fopen(filename, "wb");
	fwrite(buf, 1, len, f);
	fclose(f);
}

static int save_video(unsigned char* buf, int wrap, int xsize, int ysize, unsigned char* destbuf) {
	int total = 0;
	for (int i = 0; i < ysize; i++) {
		memcpy(destbuf+total, buf + i * wrap, xsize);
		total += xsize;
	}
	return total;
}

static void video_decode(AVCodecContext* dec_ctx, AVFrame* frame, AVPacket* pkt)
{
	int ret;
    uint64_t ts = getNowTickMs();
	ret = avcodec_send_packet(dec_ctx, pkt);
	if (ret < 0) {
		fprintf(stderr, "Error sending a packet for decoding\n");
		return;
	}
    uint64_t ts_now = getNowTickMs();
	//printf("avcodec_send_packet ms:%lld(ms)\n", (ts_now - ts));
	ts = ts_now;

	char buf[1024];
	while (ret >= 0) {
		ret = avcodec_receive_frame(dec_ctx, frame);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            //fprintf(stderr, "avcodec_receive_frame err\n");
			return;
		}
		else if (ret < 0) {
			fprintf(stderr, "Error during decoding\n");
			return;
		}
		if (ret != 0) {
			fprintf(stdout, "Error unknow\n");
			return;
		}
        ts_now = getNowTickMs();
		//printf("avcodec_receive_frame ms:%lld(ms)\n", (ts_now - ts));
		ts = ts_now;
#ifdef DEBUG_LOG
		ts = utime_ms() - g_laucnch_TS;
		//printf("recv frame %d width:%d, height:%d, %lld(ms)  \n", dec_ctx->frame_number, frame->width, frame->height, ts);
#endif
        //qDebug("recv frame %d w:%d, h:%d", dec_ctx->frame_number, frame->width, frame->height);
		if (!b_ratio_send) {
			gTrans->UpdateRatio(frame->width, frame->height);
			b_ratio_send = true;
		}
		/*if (eAVRender::g_inst->_report_resolution == false) {
			eAVRender::g_inst->_report_resolution = true;
			gTrans->SendResolution(frame->width, frame->height);
		}*/
		//fflush(stdout);

		/* the picture is allocated by the decoder. no need to
		   free it */
		//snprintf(buf, sizeof(buf), "%s-%d.yuv", "d:\\tmp\\a", 0);
		/*pgm_save(frame->data[0], frame->linesize[0], frame->width, frame->height, buf);

		int loop = frame->height / 2;
		int len_uv = frame->width / 2;//u、v分量
		*/
		bool bsucc = false;
		AVFrame* tmp_frame = nullptr;
		if (frame->format == hw_pix_fmt) {
			if ((ret = av_hwframe_transfer_data(sw_frame, frame, 0)) == 0) {
				tmp_frame = sw_frame;
				if (tmp_frame->format != AV_PIX_FMT_NV12) {
					if (sws_context == nullptr) {
						sws_context = sws_getContext(tmp_frame->width,
							tmp_frame->height,
							(AVPixelFormat)(tmp_frame->format),
							tmp_frame->width,
							tmp_frame->height,
							AV_PIX_FMT_YUV420P,
							SWS_FAST_BILINEAR,
							NULL,
							NULL,
							NULL);
					}
					int sts = 0;
					callocAVFrame(&pYUV420Frame, tmp_frame->width, tmp_frame->height);
					sts = sws_scale(sws_context,             //struct SwsContext* c,
						tmp_frame->data,        //const uint8_t* const srcSlice[],
						tmp_frame->linesize,    //const int srcStride[],
						0,                       //int srcSliceY,
						tmp_frame->height,      //int srcSliceH,
						pYUV420Frame->data,      //uint8_t* const dst[],
						pYUV420Frame->linesize); //const int dstStride[]);
					if (sts == tmp_frame->height) {
						bsucc = true;
					}
					else {
						int i = 0;
					}
				}
				else {
					pYUV420Frame = tmp_frame;
					bsucc = true;
				}
			}
		}
		else {
			pYUV420Frame = frame;
			bsucc = true;
		}

        ts_now = getNowTickMs();
		//printf("av_hwframe_transfer_data ms:%lld(ms)\n", (ts_now - ts));
		ts = ts_now;

		if (bsucc) {
			/*int loop = pYUV420Frame->height / 2;
			int len_uv = pYUV420Frame->width / 2;//u、v分量

			int yuv_len = pYUV420Frame->height * pYUV420Frame->width * 3 / 2;
			if (yuv_vec.size() != yuv_len) {
				yuv_vec.resize(yuv_len);
			}
			int bufidx = 0;
			bufidx = save_video(pYUV420Frame->data[0], pYUV420Frame->linesize[0], pYUV420Frame->width, pYUV420Frame->height, &(yuv_vec[0]));
			bufidx += save_video(pYUV420Frame->data[1], pYUV420Frame->linesize[1], len_uv, loop, &(yuv_vec[bufidx]));
			bufidx += save_video(pYUV420Frame->data[2], pYUV420Frame->linesize[2], len_uv, loop, &(yuv_vec[bufidx]));*/

			int yuv_len = av_image_get_buffer_size((AVPixelFormat)(pYUV420Frame->format), pYUV420Frame->width,
				pYUV420Frame->height, 1);
			int bufidx = yuv_len;
			if (yuv_vec.size() != yuv_len) {
				yuv_vec.resize(yuv_len);
			}
			av_image_copy_to_buffer(&(yuv_vec[0]), yuv_len,
				(const uint8_t* const*)pYUV420Frame->data,
				(const int*)pYUV420Frame->linesize, (AVPixelFormat)pYUV420Frame->format,
				pYUV420Frame->width, pYUV420Frame->height, 1);

            ts_now = getNowTickMs();
			//printf("save_video ms:%lld(ms)\n", (ts_now - ts));
			ts = ts_now;
			//std::vector<unsigned char> vec(tmpbuf, tmpbuf + bufidx);
			//vec[0] = 2;
			//vec[1] = 3;
			//vec[2] = 19;
			//raw_save(tmpbuf, bufidx, "d:\\tmp\\11.yuv");
            eVideoRender::PIXEL_FORMAT fmt = eVideoRender::PIXEL_FORMAT::RENDER_UNKNOW;
			if (pYUV420Frame->format == AV_PIX_FMT_YUV420P) {
                fmt = eVideoRender::PIXEL_FORMAT::RENDER_YUV420P;
			}
			else if (pYUV420Frame->format == AV_PIX_FMT_NV12) {
                fmt = eVideoRender::PIXEL_FORMAT::RENDER_NV12;
			}
            if (fmt != eVideoRender::PIXEL_FORMAT::RENDER_UNKNOW) {
				if (eAVRender::g_inst->_gl_render) {
					eAVRender::g_inst->_gl_render->filldata(yuv_vec, bufidx, pYUV420Frame->width, pYUV420Frame->height, fmt);
                    ts_now = getNowTickMs();
					//printf("filldata ms:%lld(ms)\n", (ts_now-ts));
					ts = ts_now;
				}
				else {
                    g_remote_render_ts = getNowTickMs();
					out_fmt = fmt;
					out_idx = bufidx;
					out_frame_w = pYUV420Frame->width;
					out_frame_h = pYUV420Frame->height;
					gTrans->SendAVData("video_data_handle", yuv_vec, bufidx, pYUV420Frame->width, pYUV420Frame->height, (int)fmt);
					if (!g_b_check_run) {
						nbase::ThreadManager::PostDelayedTask((int)ThreadId::kThreadDecode, nbase::Bind(&eAVRender::loop_check_render, eAVRender::g_inst),
							nbase::TimeDelta::FromSeconds(1));
						g_b_check_run = true;
						
					}
				}
			}
			//pgm_save(frame->data[0], frame->linesize[0], frame->width, frame->height, buf);
			//pgm_save(frame->data[1], frame->linesize[1], len_uv, loop, buf);
			//pgm_save(frame->data[2], frame->linesize[2], len_uv, loop, buf);
			//for (int i = 0; i < frame->height; i++) {
				//raw_save((unsigned char*)(frame->data[0] + i * frame->linesize[0]), frame->width, buf);
			//}
			//for (int i = 0; i < loop; i++) {
				//raw_save((unsigned char*)(frame->data[1] + i * frame->linesize[1]), len_uv, buf);
			//}
			//for (int i = 0; i < loop; i++) {
				//raw_save((unsigned char*)(frame->data[2] + i * frame->linesize[2]), len_uv, buf);
			//}
			FILE* fp;
			int i;
			int w = frame->width;
			int h = frame->height;
			//fp = fopen(buf, "ab");
			//fwrite(frame->data[0], 1, w * h, fp);//y
			//fwrite(frame->data[1], 1, w * h / 4, fp);//u
			//fwrite(frame->data[2], 1, w * h / 4, fp);//v
			//fclose(fp);
		}
	}
}


/*  function w = hanning(varargin)
%   HANNING   Hanning window.
%   HANNING(N) returns the N-point symmetric Hanning window in a column
%   vector.  Note that the first and last zero-weighted window samples
%   are not included.
%
%   HANNING(N,'symmetric') returns the same result as HANNING(N).
%
%   HANNING(N,'periodic') returns the N-point periodic Hanning window,
%   and includes the first zero-weighted window sample.
%
%   NOTE: Use the HANN function to get a Hanning window which has the
%          first and last zero-weighted samples.ep
	itype = 1 --> periodic
	itype = 0 --> symmetric
	default itype=0 (symmetric)

	Copyright 1988-2004 The MathWorks, Inc.
%   $Revision: 1.11.4.3 $  $Date: 2007/12/14 15:05:04 $
*/
float* hanning(int N, short itype)
{
	int half, i, idx, n;
	float* w;

	w = (float*)calloc(N, sizeof(float));
	memset(w, 0, N * sizeof(float));

	if (itype == 1)	//periodic function
		n = N - 1;
	else
		n = N;

	if (n % 2 == 0)
	{
		half = n / 2;
		for (i = 0; i < half; i++) //CALC_HANNING   Calculates Hanning window samples.
			w[i] = 0.5 * (1 - cos(2 * M_PI * (i + 1) / (n + 1)));

		idx = half - 1;
		for (i = half; i < n; i++) {
			w[i] = w[idx];
			idx--;
		}
	}
	else
	{
		half = (n + 1) / 2;
		for (i = 0; i < half; i++) //CALC_HANNING   Calculates Hanning window samples.
			w[i] = 0.5 * (1 - cos(2 * M_PI * (i + 1) / (n + 1)));

		idx = half - 2;
		for (i = half; i < n; i++) {
			w[i] = w[idx];
			idx--;
		}
	}

	if (itype == 1)	//periodic function
	{
		for (i = N - 1; i >= 1; i--)
			w[i] = w[i - 1];
		w[0] = 0.0;
	}
	return(w);
}

static uint8_t cache_buf[AUDIO_BUF_SIZE*10] = { 0 };
static void audio_decode(AVCodecContext* dec_ctx, AVFrame* frame, AVPacket* pkt) {
	int ret, data_size;

	ret = avcodec_send_packet(dec_ctx, pkt);
	if (ret < 0) {
		fprintf(stderr, "Error sending a packet for decoding\n");
	}

	static int nsamples = 0;
	while (ret >= 0) {
		ret = avcodec_receive_frame(dec_ctx, frame);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			return;
		else if (ret < 0) {
			fprintf(stderr, "Error during decoding\n");
		}
		data_size = av_get_bytes_per_sample(dec_ctx->sample_fmt);
		if (data_size >= 0) {
			//for (int i = 0; i < frame->nb_samples; i++) {
				//printf("recv frame %d\n", dec_ctx->frame_number);
				//fflush(stdout);
			//}
			//int totalsize = frame->nb_samples * dec_ctx->channels * data_size;
			//eAVRender::g_inst->writeAudioRingBuf(frame->data[0], totalsize);

			/*for (int i = 0; i < frame->nb_samples; i++) {
				for (int ch = 0; ch < frame->channels; ch++) {
					DWORD dwsize = 0;
					//WriteFile(eAVRender::g_inst->_fp, frame->data[ch] + data_size * i, data_size, &dwsize, nullptr);
				}
			}*/

			int dst_nb_samples = av_rescale_rnd(swr_get_delay(g_audio_dec_ctx->convert_audio_ctx, frame->sample_rate) +  frame->nb_samples,
				AUDIO_PLAY_SAMPLE,//得到变换后采用点数
				frame->sample_rate, AV_ROUND_INF);
			//audio_out_buffer
			uint8_t* output;
			int err = av_samples_alloc(&output, NULL, 2, dst_nb_samples,
				AUDIO_DATA_TYPE, 0);
			int len = swr_convert(g_audio_dec_ctx->convert_audio_ctx, &output, dst_nb_samples, (const uint8_t**)frame->data, frame->nb_samples);   //变换器启动
			//audio_buf = *audio_out_buffer;  //获得输出数据
			int audio_len = len * av_get_bytes_per_sample(AUDIO_DATA_TYPE) * 2;	//获取输出音频数据大小

#ifdef DEBUG_LOG
			//printf("audio dst_nb_samples:%d, outsample:%d, audio_len:%d \n", dst_nb_samples, len, audio_len);
#endif
            unsigned long dwsize = 0;
			//WriteFile(eAVRender::g_inst->_fp, output, audio_len, &dwsize, nullptr);
			eAVRender::g_inst->saveaudio(output, audio_len);
			static int n_cache_times = 0;
			static int cache_size = 0;
			static int cache_samples = 40; //缓存样本数

			/*unsigned int acture_len = eAVRender::g_inst->audio_buf_len();
			if (acture_len < audio_len * cache_samples  && n_cache_times <= 0) {
				n_cache_times = 0;
			}*/

			if (n_cache_times < cache_samples) {
				memcpy(cache_buf + cache_size, output, audio_len);
				cache_size += audio_len;
				++n_cache_times;
			}
			else {
				//cache_samples = 10;
				//n_cache_times = 0;
				if (cache_size > 0) {
					eAVRender::g_inst->writeAudioRingBuf(cache_buf, cache_size);
					cache_size = 0;
				}
				else {
					eAVRender::g_inst->writeAudioRingBuf(output, audio_len);
				}
			}
			av_freep(&output);


		}
		//fflush(stdout);
	}
}

void DestoryDecode()
{
	if (g_video_dec_ctx != NULL)
	{
		if (g_video_dec_ctx->convert_ctx) {
			sws_freeContext(g_video_dec_ctx->convert_ctx);
			g_video_dec_ctx->convert_ctx = NULL;
		}
		avcodec_close(g_video_dec_ctx->codec_ctx);
		av_free(g_video_dec_ctx->codec_ctx);
		av_free(g_video_dec_ctx->src_frame);
		av_free(g_video_dec_ctx->dst_frame);
		free(g_video_dec_ctx);
	}
}


void InitOpus() {
	g_audio_dec_ctx = (DecoderContext*)calloc(1, sizeof(DecoderContext));
	g_audio_dec_ctx->color_format = 0;

	g_audio_dec_ctx->codec = const_cast<AVCodec*>(avcodec_find_decoder(AV_CODEC_ID_OPUS));
	//av_parser_init(g_audio_dec_ctx->codec->id);
	g_audio_dec_ctx->codec_ctx = avcodec_alloc_context3(g_audio_dec_ctx->codec);
	//g_audio_dec_ctx->codec_ctx->flags2 |= AV_CODEC_FLAG2_CHUNKS;
	//g_audio_dec_ctx->codec_ctx->flags |= AV_CODEC_FLAG_LOW_DELAY;
	g_audio_dec_ctx->codec_ctx->sample_rate = AUDIO_SAMPLE;
	//g_audio_dec_ctx->codec_ctx->sample_fmt = AV_SAMPLE_FMT_FLT; // AV_SAMPLE_FMT_FLT; AV_SAMPLE_FMT_S16
	g_audio_dec_ctx->codec_ctx->channels = 2;
	if (g_audio_dec_ctx->codec->capabilities & AV_CODEC_CAP_TRUNCATED)
		g_audio_dec_ctx->codec_ctx->flags |= AV_CODEC_FLAG_TRUNCATED;


	if (avcodec_open2(g_audio_dec_ctx->codec_ctx, g_audio_dec_ctx->codec, NULL) < 0)
	{
		printf("avcodec open error\n");
		return;
	}

	g_audio_dec_ctx->convert_audio_ctx = swr_alloc();
	//下面设置重新采样输入和输出参数
	g_audio_dec_ctx->convert_audio_ctx = swr_alloc_set_opts(NULL,
		av_get_default_channel_layout(2),//输出
		AUDIO_DATA_TYPE, //编码前你希望的格式
		AUDIO_PLAY_SAMPLE,//输出
		av_get_default_channel_layout(g_audio_dec_ctx->codec_ctx->channels), //in_channel_layout, //输入
		g_audio_dec_ctx->codec_ctx->sample_fmt,//PCM源文件的采样格式
		g_audio_dec_ctx->codec_ctx->sample_rate, //输入
		0, 0);
	int err = swr_init(g_audio_dec_ctx->convert_audio_ctx);

	g_audio_dec_ctx->src_frame = av_frame_alloc();
	g_audio_dec_ctx->dst_frame = av_frame_alloc();
	return;
}

bool isIFrame(const unsigned char* data, const int dsize, bool bh265) {
	bool biframe = false;
	unsigned char kdata = 0;
	if (bh265) {
		kdata = (data[4] >> 1) & 0x3f;
		if (kdata == 19 || kdata == 32) {
			biframe = true;
		}
	}
	else {
		kdata = data[4] & 0x1f;
		if (kdata == 0x05 || kdata == 0x07) {
			biframe = true;
		}
	}
	printf("is frame: %d\n", biframe);
	return biframe;
}

void eAVRender::DecodeVideo(std::vector<unsigned char> data) {
	if (!b_play_done) {
		return;
	}
	if (bStopRender) {
		return;
	}
	static int n = 0;
	unsigned char* rawdata = &(data[0]);
    long long ts = getNowTickMs();
	DecodeH264((char*)rawdata, data.size(), n++);
    long long diff = getNowTickMs() - ts;
	//printf("DecodeVideo: %lld(ms)\n", diff);
}

void setbitrate(bool bauto, int bitrate) {
	nlohmann::json root;
	root["cmd"] = 5000;
	if (bauto) {
		root["param"] = 1;
	}
	else {
		root["param"] = 0;
	}
	pYJNetVideoControl(const_cast<char*>(root.dump().c_str()));

	if (bauto == false) {
		root["cmd"] = 5001;
		root["param"] = bitrate;
	}
	pYJNetVideoControl(const_cast<char*>(root.dump().c_str()));
	fprintf(stdout, "#####------- auto: %d, rate: %d\n", g_bAutorate, g_Bitrate);
}

void setfps(int fps) {
	nlohmann::json root;
	root["cmd"] = 5002;
	root["param"] = fps;
	pYJNetVideoControl(const_cast<char*>(root.dump().c_str()));
	fprintf(stdout, "#####------- fps: %d\n", fps);
}

bool bfirst_v_frame = false;
bool bupate_quality = false;
void videoCB(void* data, int len, uint64_t) {
	static int n = 1;
	static bool bfirst = true;
	if (bfirst) {
		bfirst = false;
        uint64_t ts = getNowTickMs() - g_laucnch_TS;
		printf("##---connect media succ: %lld(ms)\n", ts);
	}
	
	eAVRender::g_inst->savevideo((unsigned char*)data, len);
    //printLogMsg("render", "videoCB: %u, len:%d", n, len);
	if (!(a_v_render_state & VIDEO_RENDER_STATE_OK)) {
		fprintf(stderr, "ffmpeg no ready\n");
		return;
	}
	//debug info
	/*{
		std::string strts = GetCurrentTimeStamp(1);
		printf("recv_cb :%s\n", strts.c_str());
		//return;
	}
	{
		static int times = 1;
		//static uint64_t check_ts = utime_ms();
		static uint64_t next_ts = 0;
		static bool in_sec = false;
		if (in_sec == false) {
			next_ts = utime_ms() + 1000;
			in_sec = true;
			times = 1;
		}
		++times;
		if (in_sec) {
			uint64_t now_ts = utime_ms();
			if (now_ts >= next_ts) {
				in_sec = false;
				printf("########recv frames: %d\n", times);
			}
		}

	}*/
	if (!gDone) {
		static bool bhand_kframe = true;
		if (!bhand_kframe) {
			bhand_kframe = isIFrame((const unsigned char*)data, len, b_h265);
		}
		if (bhand_kframe) {
			std::vector<unsigned char> rawdata((unsigned char*)data, (unsigned char*)data + len);
			nbase::ThreadManager::PostTask((int)ThreadId::kThreadDecode, nbase::Bind(&eAVRender::DecodeVideo, eAVRender::g_inst, rawdata));
		}
		else {
			fprintf(stderr, "not i frame\n");
		}
		//DecodeH264((char*)data, len, n++);
		if (!bfirst_v_frame) {
			gTrans->SendCollectData(std::string("connect_media_log"), GenCollectStr("got first frame"), true);
			gTrans->SetConnState(AV_CONN);
			//uint64_t ts = utime_ms() - g_laucnch_TS;
			//printf("###---connect media succ: %lld\n", getSysTime());
			//printf("###---connect media succ: %lld(ms)\n", ts);
			bfirst_v_frame = true;
			setbitrate(g_bAutorate, g_Bitrate);
			gTrans->SendFrameActivate();
		}
		if (bupate_quality) {
			bupate_quality = false;
			//setbitrate(g_bAutorate, g_Bitrate);
			//fprintf(stdout, "#####------- auto: %d, rate: %d\n", g_bAutorate, g_Bitrate);
		}
		++n;
	}
}

void audioCB(void* data, int len, uint64_t) {
	static unsigned int n = 0;
	//printLogMsg("render", "audioCB: %u, len: %d", n, len);
	if (!(a_v_render_state & AUDIO_RENDER_STATE_OK)) {
		return;
	}
	if (!gDone) {
		//eAVRender::g_inst->saveaudio((unsigned char*)data, len);
		DecodeOpus((char*)data, len, n++);
	}
}

void msgCB(void* data, int len) {
    char* msg = (char*)malloc(len + 1);
	memset(msg, 0, len + 1);
	memcpy(msg, (char*)data, len);
    //printLogMsg("render", "msgCB:(len=%d) %s", len, msg);

	std::string strmsg("msgcb:");
//#ifdef _DEBUG
	strmsg.append(msg);
	strmsg.append("\n");
	
    qDebug(strmsg.c_str());
//#endif
    //int i = 0;
	//type:"6000", msg:"Disconnected"
	eAVRender::g_inst->savelog(strmsg.c_str());
	static bool bcollect = false;
	if (!bfirst_v_frame) {
		bcollect = true;
		gTrans->SendCollectData(std::string("connect_media_log"), GenCollectStr(strmsg), true);
	}

	int code = 0;
	nlohmann::json root;
	try {
		root = nlohmann::json::parse(msg, nullptr, true);
		if (root.find("type") != root.end() && !root.at("type").is_null()) {
			root.at("type").get_to(code);
		}
		if (code == 6002 || code == 6003 || code == 6004 || code == 6005) {
			//eAVRender::g_inst->helpReconnect();
			if (!bcollect) {
				//nlohmann::json parm;
				//parm["code"] = code;
				//gTrans->SendCollectData(std::string("connect_media_error"), parm.dump());
                //gTrans->SendCollectData(std::string("connect_media_log"), GenCollectStr(strmsg), true);
			}
			gTrans->RemoveConnState(AV_CONN);
			int type = EXIT_NETWORK;
			if (g_multi_win_platform) {
				//直接通知主程序退出窗口
				type = EXIT_MULTI;
			}
			gTrans->goDisconnect("jm_disconnect", type);
		}
		else if (code == 6001) {
            uint64_t ts = getNowTickMs() - g_laucnch_TS;
			printf("###---6001: %lld(ms)\n", ts);
		}
	}
	catch (...) {

	}
	
	free(msg);
}

//172.27.11.29 211.155.101.172
unsigned int gotAVData(void* arg, const char* orderid, const char* token, const char* guid,  const char* av_addr, const char* ws_addr, const char* stun_addr, int connect_type, int data_type) {
	const char* json = R"myjson({
    	"wsserveraddr":"ws://172.27.11.28:21009",
		"tcperveraddr":"",
		"stunserveraddr":"stun:172.27.11.28:3478",
		"serverMsg":"172.27.11.28:17005",
		"orderid":"1238882",
		"apptype":"yunphone",
		"expand":"",
		"param":"{\"datatype\":2,\"width\":1920,\"height\":1080,\"bitrate\":3000,\"fps\":30,\"token\":\"123\",\"phoneguid\":\"4casdf1a1236788343da345342123\"}",
		"ismgr":1,
		"videotype":1,
		"conntype":1
    })myjson";
	nlohmann::json root;
	root = nlohmann::json::parse(json, nullptr, true);
	if (connect_type == 0) {
		root["tcpserveraddr"] = av_addr;
		root["apptype"] = "websocket";
		root["serverMsg"] = av_addr;
		root["stunserveraddr"] = stun_addr;
		root["wsserveraddr"] = ws_addr;
	}
	else if(connect_type == 1) {
		root["serverMsg"] = av_addr;
		root["wsserveraddr"] = ws_addr;
		root["stunserveraddr"] = stun_addr;
		root["apptype"] = "yunphone";
	}
	root["conntype"] = 1;
	root["videotype"] = ((data_type % 2) == 0) ? 1 : 0; //0 h264, 1 h265
	srand(time(0));
	//int orderid = rand() % 5238882 + 1;
	//char szorderid[64] = { 0 };
	//sprintf_s(szorderid, 63, "%s", orderid);
    sprintf(szorderid, "%d_%lld", g_order_id, getUtcTickms());

	//std::wstring app_dir = nbase::win32::GetCurrentModuleDirectory();
	//app_dir.append(L"video.ini");
	//std::string inipath = nbase::UTF16ToUTF8(app_dir);
	int bitrate = g_Bitrate;// GetPrivateProfileIntA("video", "bitrate", 3000, inipath.c_str());
	nlohmann::json parm;
	parm["datatype"] = data_type;
	parm["width"] = g_width;
	parm["height"] = g_height;
	parm["bitrate"] = bitrate;
	parm["fps"] = g_fps;
	parm["token"] = token;
	parm["phoneguid"] = guid;
	root["param"] = parm.dump().c_str();
	root["orderid"] = szorderid;

	std::string pparm = root.dump();
    uint64_t ts = getNowTickMs() - g_laucnch_TS;
	printf("########-----video parm: %s, %lld(ms)\n", pparm.c_str(), ts);
    //printLogMsg("render", "video parm: %s, %lld(ms)", pparm.c_str(), ts);
	const char* ss = pparm.c_str();
    pYJNetInit(videoCB, audioCB, msgCB);
    pYJNetConnect(const_cast<char*>(pparm.c_str()));
    //printLogMsg("render", "call pYJNetConnect done");
    return 0;
}

void eAVRender::writeAudioRingBuf(unsigned char* buf, int len) {
	static unsigned char grab[AUDIO_BUF_SIZE];
    unsigned long dwsize = 0;
	//WriteFile(_fp, buf, len, &dwsize, nullptr);
	//std::unique_lock<std::mutex> lock(_audio_mutex);
    //EnterCriticalSection(&_audio_cs);
    std::unique_lock<std::mutex> lock(_audio_cs);
	int remain = ring_buf_remain(_audio_ring_buf);
	if (len > remain) {
		printf("######writeAudioRingBuf no enough!!!!! \n");
		int newlen = _audio_ring_buf->size + remain + len;
		ring_buf_t* new_ring_buf = new ring_buf_t;
		unsigned char* new_buf = new unsigned char[newlen];
		ring_buf_realloc(_audio_ring_buf, new_ring_buf, new_buf, newlen);
		delete _audio_ring_buf;
		_audio_ring_buf = nullptr;
		delete _audio_buf;
		_audio_buf = nullptr;
		_audio_ring_buf = new_ring_buf;
		_audio_buf = new_buf;
		//ring_buf_put(_audio_ring_buf, buf, len);
		//ring_buf_get(_audio_ring_buf, grab, len);
		
		//ring_buf_put()

	}
	ring_buf_put(_audio_ring_buf, buf, len);
    //LeaveCriticalSection(&_audio_cs);
}

void eAVRender::readAudioRingBuf(unsigned char* outbuf, int& outlen, int maxlen) {
	//std::unique_lock<std::mutex> lock(_audio_mutex);
    //EnterCriticalSection(&_audio_cs);
    std::unique_lock<std::mutex> lock(_audio_cs);
	unsigned int acture_len = ring_buf_len(_audio_ring_buf);
	outlen = acture_len > maxlen ? maxlen : acture_len;
	outlen = ring_buf_get(_audio_ring_buf, outbuf, outlen);
    //LeaveCriticalSection(&_audio_cs);
}

unsigned int eAVRender::audio_buf_len() {
	//std::unique_lock<std::mutex> lock(_audio_mutex);
	unsigned int nsize = 0;
    //EnterCriticalSection(&_audio_cs);
    std::unique_lock<std::mutex> lock(_audio_cs);
	nsize = ring_buf_len(_audio_ring_buf);
    //LeaveCriticalSection(&_audio_cs);
	return nsize;
}

void eAVRender::savevideo(unsigned char* buf, int len) {
    /*std::wstring app_dir = nbase::win32::GetCurrentModuleDirectory();
	std::wstring debug_path = app_dir;
	debug_path.append(L"_debug");
	if (-1 == _waccess(debug_path.c_str(), 0)) {
		return;
	}
	if (_video_fp == nullptr) {
		std::wstring dmp_video;
		dmp_video = nbase::StringPrintf(L"%s\\video_%s.dmp", app_dir.c_str(), nbase::UTF8ToUTF16(szorderid).c_str());
		std::string dmp_video_path = Unicode2ASCI(dmp_video.c_str());
		fopen_s(&_video_fp, dmp_video_path.c_str(), "wb");
	}
	if (_video_fp) {
		fwrite(buf, 1, len, _video_fp);
    }*/
}

void eAVRender::saveaudio(unsigned char* buf, int len) {
    /*std::wstring app_dir = nbase::win32::GetCurrentModuleDirectory();
	std::wstring debug_path = app_dir;
	debug_path.append(L"_debug");
	if (-1 == _waccess(debug_path.c_str(), 0)) {
		return;
	}
	if (_audio_fp == nullptr) {
		std::wstring dmp_video = app_dir;
		dmp_video.append(L"audio.pcm");
		std::string dmp_video_path = Unicode2ASCI(dmp_video.c_str());
		fopen_s(&_audio_fp, dmp_video_path.c_str(), "wb");
	}
	if (_audio_fp) {
		fwrite(buf, 1, len, _audio_fp);
    }*/
}

void eAVRender::savelog(const char* log) {
    /*std::wstring app_dir = nbase::win32::GetCurrentModuleDirectory();
	std::wstring debug_path = app_dir;
	debug_path.append(L"_debug");
	if (-1 == _waccess(debug_path.c_str(), 0)) {
		return;
	}
	if (_log_fp == nullptr) {
		std::wstring log = app_dir;
		log.append(L"msgcb_log.txt");
		std::string log_path = Unicode2ASCI(log.c_str());
		fopen_s(&_log_fp, log_path.c_str(), "wb");
	}
	if (_log_fp) {
		fwrite(log, 1, strlen(log), _log_fp);
    }*/
}

void eAVRender::setParm(const char* av_addr, const char* ws_addr, const char* stun_addr, const int connect_type, const int data_type) {
	_str_av_addr = av_addr;
	_str_ws_addr = ws_addr;
	_str_stun_addr = stun_addr;
	_connect_type = connect_type;
	_data_type = data_type;
	b_h265 = (data_type % 2) == 0;
	//bHWacc = b_h265;
};

void eAVRender::SetExtInfo(const char* token, const char* guid,
                           const void* glwnd, const int init_wdith, const int init_height) {
	_token = token;
	_guid = guid;
    _gl_hWnd = (long)glwnd;
    _gl_width = init_wdith;
    _gl_height = init_height;
}

unsigned int eAVRender::streamConnect(void* arg) {
	eAVRender* self = (eAVRender*)arg;
    char szDll[512] = { 0 };
    sprintf(szDll, "%s/libyjrtc.so", GetExeDirectory().c_str());
    self->_hmod = dlopen(szDll, RTLD_LAZY);
	if (self->_hmod) {
        pYJNetInit = (FnYJNetInit)dlsym(self->_hmod, "YJNetInit");
        pYJNetConnect = (FnYJNetConnect)dlsym(self->_hmod, "YJNetConnect");
        pYJNetDisConnect = (FnYJNetDisConnect)dlsym(self->_hmod, "YJNetDisConnect");
        pYJNetVideoControl = (FnYJNetVideoControl)dlsym(self->_hmod, "YJNetVideoControl");
		unsigned int dwThreadID = 0;
		if (pYJNetInit && pYJNetConnect && pYJNetDisConnect && pYJNetVideoControl) {
            //self->_audio_buf = new unsigned char[AUDIO_BUF_SIZE];
            //self->_audio_ring_buf = new ring_buf_t;
            //ring_buf_create(self->_audio_ring_buf, self->_audio_buf, AUDIO_BUF_SIZE);
            //printLogMsg("render", "call gotAVData");
			gotAVData(self, self->_orderid.c_str(), self->_token.c_str(), self->_guid.c_str(),
				self->_str_av_addr.c_str(), self->_str_ws_addr.c_str(), self->_str_stun_addr.c_str(), self->_connect_type, self->_data_type);
		}
	}
	return 0;
}

void eAVRender::Init() {
	if (!g_play_logo_animation) {
		b_play_done = true;
	}
    _audio_buf = new unsigned char[AUDIO_BUF_SIZE];
    _audio_ring_buf = new ring_buf_t;
    ring_buf_create(_audio_ring_buf, _audio_buf, AUDIO_BUF_SIZE);
	InitH264Decode();
	a_v_render_state = VIDEO_RENDER_STATE_OK;
	if (_b_init_with_connectStream) {
        //unsigned int dwThreadID;
        //HANDLE hThd = (HANDLE)_beginthreadex(nullptr, 0, eAVRender::streamConnect, this, 0, &dwThreadID);
        //if (hThd) {
        //	CloseHandle(hThd);
        //}
        //std::thread thd(eAVRender::streamConnect, this);
        //thd.detach();
        triggerStraamThread();
	}
	//eAVRender::streamConnect(this);

	uint64_t ts = 0;
    //nbase::ThreadManager::RegisterThread((int)thread_id_);
	eAVRender::g_inst = this;
    //InitializeCriticalSection(&_audio_cs);

    ts = getNowTickMs() - g_laucnch_TS;
	printf("####=====eAVRender::Init glwnd: %p, %lld(ms)\n", _gl_hWnd, ts);
    //if (IsWindow(_gl_hWnd) == false) {
    //	return;
    //}
	//Sleep(20000);
    //RECT rc;
    //GetWindowRect(_gl_hWnd, &rc);
    _gl_render = eVideoRender::Create((void*)_gl_hWnd, _gl_width , _gl_height, g_angle);
	//printf("####=====eAVRender:: eGLRender: %d\n", _gl_render);
	
    ts = getNowTickMs() - g_laucnch_TS;
	printf("###---eGLRender::Create ok: %lld(ms)\n", ts);

	openSDL();
	InitOpus();
	a_v_render_state |= AUDIO_RENDER_STATE_OK;
	if (g_play_logo_animation) {
		nbase::ThreadManager::PostTask((int)ThreadId::kThreadDecode, nbase::Bind(&eAVRender::PlayFile, eAVRender::g_inst));
	}

	/*
    wchar_t szDll[MAX_PATH] = {0};
    wsprintf(szDll, L"%s\\libyjrtc.dll", getSelfExePath().c_str());
    _hmod = LoadLibrary(szDll);
    if (_hmod) {
        pYJNetInit = (FnYJNetInit)GetProcAddress(_hmod, "YJNetInit");
        pYJNetConnect = (FnYJNetConnect)GetProcAddress(_hmod, "YJNetConnect");
        pYJNetDisConnect = (FnYJNetDisConnect)GetProcAddress(_hmod, "YJNetDisConnect");
		pYJNetVideoControl = (FnYJNetVideoControl)GetProcAddress(_hmod, "YJNetVideoControl");
        unsigned int dwThreadID = 0;
        if (pYJNetInit && pYJNetConnect && pYJNetDisConnect && pYJNetVideoControl) {
            //HANDLE hThd = (HANDLE)_beginthreadex(nullptr, 0, gotAVData, this, 0, &dwThreadID);
            //if(hThd)
            //    CloseHandle(hThd);

			//_fp = CreateFile(L"d:\\tmp\\pc.pcm", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			_audio_buf = new unsigned char[AUDIO_BUF_SIZE];
			_audio_ring_buf = new ring_buf_t;
			ring_buf_create(_audio_ring_buf, _audio_buf, AUDIO_BUF_SIZE);
			openSDL();
			//openAucioPlayClient();
			InitH264Decode();
			InitOpus();
			printLogMsg("render", "call gotAVData");
            gotAVData(this, _orderid.c_str(), _token.c_str(), _guid.c_str(), _str_av_addr.c_str(), _str_ws_addr.c_str(), _str_stun_addr.c_str(), _connect_type, _data_type);
        }
    }
	*/
}

void eAVRender::helpReconnect() {
	srand(time(0));
    nbase::StdClosure fn = this->ToWeakCallback(nbase::Bind([=]()->void {
		gotAVData(this, _orderid.c_str(), _token.c_str(), _guid.c_str(), _str_av_addr.c_str(), _str_ws_addr.c_str(), _str_stun_addr.c_str(), _connect_type, _data_type);
        }));
    nbase::ThreadManager::PostDelayedTask((int)thread_id_, fn,
		nbase::TimeDelta::FromSeconds(rand() % 12 + 1));
}

unsigned int av_disconnect(void* arg) {
	if (pYJNetDisConnect) {
		pYJNetDisConnect();
	}
	return 0;
}

void eAVRender::Cleanup() {
	if (_gl_render.get()) {
        _gl_render->Close();
	}
	unsigned int dwThreadID = 0;
	if (_video_fp) {
		fclose(_video_fp);
	}
	if (_log_fp) {
		fclose(_log_fp);
	}
	if (_audio_fp) {
		fclose(_audio_fp);
	}
    //HANDLE hThd = (HANDLE)_beginthreadex(nullptr, 0, av_disconnect, this, 0, &dwThreadID);
    //if (hThd) {
    //	WaitForSingleObject(hThd, 3000);
    //	CloseHandle(hThd);
    //}
    std::thread thd(av_disconnect, this);
    thd.detach();
    usleep(1000);
	printf("Cleanup \n");
    //SetThreadWasQuitProperly(false);
    //nbase::ThreadManager::UnregisterThread();
}

//位深度
#define SAMPLE_BIT_DEPTH 16
//声道数
#define CHANNELS 2
//音频缓存区的样本大小
//#define SAMPLES 4096
#define SAMPLES AUDIO_PLAY_SAMPLE * CHANNELS / 1000 * 40
//每个样本占多少字节 (1byte = 8bit)
#define SAMPLE_PER_BYTES ((SAMPLE_BIT_DEPTH * CHANNELS) / 8)
//文件缓冲区的大小 (样本大小 *  每个样本占多少字节)
#define BUFFER_SIZE (SAMPLES * SAMPLE_PER_BYTES)

int eAVRender::openSDL() {
	int err = SDL_Init(SDL_INIT_EVERYTHING);
	SDL_AudioSpec* wanted_spec;
	int wanted_nb_channels = CHANNELS;
	int samplerate = AUDIO_PLAY_SAMPLE;
	//   int samplerate = out_sample_rate;
	wanted_spec = (SDL_AudioSpec*)SDL_malloc(sizeof(struct SDL_AudioSpec));
	wanted_spec->channels = wanted_nb_channels;
	wanted_spec->samples = SAMPLES;
	wanted_spec->freq = samplerate;
	wanted_spec->format = AUDIO_S16LSB; // 具体含义请查看“SDL宏定义”部分
	wanted_spec->silence = 0;            // 0指示静音
    //wanted_spec->samples = SDL_AUDIO_BUFFER_SIZE;  // 自定义SDL缓冲区大小
	wanted_spec->size = BUFFER_SIZE;
	wanted_spec->callback = sdlAudioCallBackFunc;  // 回调函数
	wanted_spec->userdata = g_inst;                  // 传给上面回调函数的外带数据
	//打开音频设备，第二个参数为播放音频的设备，NULL表示默认设备
	if (SDL_OpenAudio(wanted_spec, nullptr) < 0) {
		printf("打开SDL Audio 失败\n");
		return -1;
	}
	SDL_PauseAudio(0);
	return 0;
}

unsigned char g_audio[AUDIO_BUF_SIZE];
int actrue_len = 0;
void eAVRender::sdlAudioCallBackFunc(void* userdata, unsigned char* stream, int len) {
	eAVRender* self = (eAVRender*)userdata;
	SDL_memset(stream, 0, len);
	static int times = 0;
	//static bool bfirst = true;
	/*if (bfirst) {
		//memset(g_audio, 0, AUDIO_BUF_SIZE);
		self->readAudioRingBuf(g_audio, actrue_len, len);
		bfirst = false;
	}
	else {
		self->readAudioRingBuf(g_audio, actrue_len, len);
	}*/
	self->readAudioRingBuf(g_audio, actrue_len, len);
	int try_times = 0;
	while (actrue_len == 0) {
		if (try_times > 2) {
			break;
		}
		++try_times;
#ifdef DEBUG_LOG
		char szmsg[100] = { 0 };
		//sprintf(szmsg, "audio read size: %d - %d, try: %d \n", actrue_len, ++times, try_times);
		//printf(szmsg);
#endif
		SDL_Delay(5);
		self->readAudioRingBuf(g_audio, actrue_len, len);
	}

	if (actrue_len > 0) {
		SDL_MixAudio(stream, g_audio, actrue_len, 80);
		//printf("play audio acturelen:%d, len: %d \n", actrue_len, len);
		//fflush(stdout);
		//SDL_Delay(0);
		//WriteFile(eAVRender::g_inst->_fp, g_audio, actrue_len, &dwsize, nullptr);
	}
	actrue_len = 0;
	//int actrue_len = 0;
	//memset(g_audio, 0, AUDIO_BUF_SIZE);
	//self->readAudioRingBuf(g_audio, actrue_len, len);
	//SDL_Delay(10);
}

bool eAVRender::openAucioPlayClient() {
    /*bool r = _audioRender.Open();
	if (r) {
		_bytePreSecLen = _audioRender.getFmt()->nAvgBytesPerSec;
		_audioRender.SetCB(this, &eAVRender::msAudioCBFunc);
	}
    return r;*/
    return true;
}

void eAVRender::msAudioCBFunc(unsigned char* pData, unsigned int nSmaples, unsigned int nChn, void* userdata) {
	eAVRender* self = (eAVRender*)userdata;
	int dsize = sizeof(float)* nSmaples* nChn;
	/*static bool bfirst = true;
	if (bfirst) {
		bfirst = false;
		self->readAudioRingBuf(g_audio, actrue_len, dsize);
	}
	memcpy(pData, g_audio, dsize);
	self->readAudioRingBuf(g_audio, actrue_len, dsize);*/

	self->readAudioRingBuf(pData, actrue_len, dsize);
	printf("sample:%d , actrue_len: %d\n", nSmaples, actrue_len);

}

void eAVRender::StopVideo() {
	unsigned int dwThreadID = 0;
    /*HANDLE hThd = (HANDLE)_beginthreadex(nullptr, 0, av_disconnect, this, 0, &dwThreadID);
	if (hThd) {
		//WaitForSingleObject(hThd, 3000);
		CloseHandle(hThd);
    }*/
    std::thread thd(av_disconnect, this);
    thd.detach();
    usleep(1000);
}

void eAVRender::triggerStraamThread() {
	unsigned int dwThreadID;
    /*HANDLE hThd = (HANDLE)_beginthreadex(nullptr, 0, eAVRender::streamConnect, this, 0, &dwThreadID);
	if (hThd) {
		CloseHandle(hThd);
    }*/
    std::thread thd(eAVRender::streamConnect, this);
    thd.detach();
}

void eAVRender::loop_check_render() {
    uint64_t now = getNowTickMs();
	if (now - g_remote_render_ts > 1000) {
		g_remote_render_ts = now;
		gTrans->SendAVData("video_data_handle", yuv_vec, out_idx, out_frame_w, out_frame_h, (int)out_fmt);
	}
	nbase::ThreadManager::PostDelayedTask((int)ThreadId::kThreadDecode, nbase::Bind(&eAVRender::loop_check_render, eAVRender::g_inst),
		nbase::TimeDelta::FromSeconds(1));
}

static AVFormatContext* fmt_ctx;
static AVCodecContext* video_dec_ctx;
static AVCodecContext* audio_dec_ctx;
static int video_stream_index = -1;
static int audio_stream_index = -1;
static int64_t last_pts = AV_NOPTS_VALUE;

static int open_input_file(const char* filename)
{
	const AVCodec* video_dec;
	const AVCodec* audio_dec;
	int ret;

	if ((ret = avformat_open_input(&fmt_ctx, filename, NULL, NULL)) < 0) {
		av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
		return ret;
	}

	if ((ret = avformat_find_stream_info(fmt_ctx, NULL)) < 0) {
		av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
		return ret;
	}

    // select the video stream
	ret = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &video_dec, 0);
	if (ret < 0) {
		av_log(NULL, AV_LOG_ERROR, "Cannot find a video stream in the input file\n");
		return ret;
	}
	video_stream_index = ret;

    // create decoding context
	video_dec_ctx = avcodec_alloc_context3(video_dec);
	if (!video_dec_ctx)
		return AVERROR(ENOMEM);
	avcodec_parameters_to_context(video_dec_ctx, fmt_ctx->streams[video_stream_index]->codecpar);

    // init the video decoder
	if ((ret = avcodec_open2(video_dec_ctx, video_dec, NULL)) < 0) {
		av_log(NULL, AV_LOG_ERROR, "Cannot open video decoder\n");
		return ret;
	}

    // select the audio stream
	ret = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, &audio_dec, 0);
	if (ret < 0) {
		av_log(NULL, AV_LOG_ERROR, "Cannot find a audio stream in the input file\n");
		return ret;
	}
	audio_stream_index = ret;

    // create decoding context
	audio_dec_ctx = avcodec_alloc_context3(audio_dec);
	if (!audio_dec_ctx)
		return AVERROR(ENOMEM);
	avcodec_parameters_to_context(audio_dec_ctx, fmt_ctx->streams[audio_stream_index]->codecpar);

    // init the video decoder
	if ((ret = avcodec_open2(audio_dec_ctx, audio_dec, NULL)) < 0) {
		av_log(NULL, AV_LOG_ERROR, "Cannot open audio decoder\n");
		return ret;
	}
	return 0;
}

int eAVRender::PlayFile() {
	printf("playfile\n");
	int ret;
	AVPacket* packet;
	AVFrame* frame;
	AVFrame* filt_frame;
    std::string str_dir;
    std::string str_asci;
    char sz[512] = {0};

	frame = av_frame_alloc();
	filt_frame = av_frame_alloc();
	packet = av_packet_alloc();
	if (!frame || !filt_frame || !packet) {
		fprintf(stderr, "Could not allocate frame or packet\n");
		return 1;
	}

	
    //std::wstring root_path = nbase::win32::GetCurrentModuleDirectory();
    //std::wstring mp4_path = nbase::StringPrintf(L"%s\\..\\rh.mp4", root_path.c_str());
    //std::string str_mp4 = nbase::UTF16ToUTF8(mp4_path);
    //std::string str_asci = UTF8toASCI(str_mp4.c_str());
    str_dir = GetExeDirectory();
    sprintf(sz, "%s/info.mp4", str_dir.c_str());
    if ((ret = open_input_file(sz)) < 0) {
		goto end;
	}
	//if ((ret = init_filters(filter_descr)) < 0) {
	//	goto end;
	//}

    // read all packets
	while (1) {
		if ((ret = av_read_frame(fmt_ctx, packet)) < 0)
			break;

		if (packet->stream_index == video_stream_index) {
			ret = avcodec_send_packet(video_dec_ctx, packet);
			if (ret < 0) {
				av_log(NULL, AV_LOG_ERROR, "Error while sending a packet to the decoder\n");
				break;
			}

			while (ret >= 0) {
				ret = avcodec_receive_frame(video_dec_ctx, frame);
				if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
					break;
				}
				else if (ret < 0) {
					av_log(NULL, AV_LOG_ERROR, "Error while receiving a frame from the decoder\n");
					goto end;
				}

				frame->pts = frame->best_effort_timestamp;

                av_usleep(22);
				{
					//play
					bool bsucc = false;
					AVFrame* tmp_frame = nullptr;
					if (frame->format == hw_pix_fmt) {
						if ((ret = av_hwframe_transfer_data(sw_frame, frame, 0)) == 0) {
							tmp_frame = sw_frame;
							if (tmp_frame->format != AV_PIX_FMT_NV12) {
								if (sws_context == nullptr) {
									sws_context = sws_getContext(tmp_frame->width,
										tmp_frame->height,
										(AVPixelFormat)(tmp_frame->format),
										tmp_frame->width,
										tmp_frame->height,
										AV_PIX_FMT_YUV420P,
										SWS_FAST_BILINEAR,
										NULL,
										NULL,
										NULL);
								}
								int sts = 0;
								callocAVFrame(&pYUV420Frame, tmp_frame->width, tmp_frame->height);
								sts = sws_scale(sws_context,             //struct SwsContext* c,
									tmp_frame->data,        //const uint8_t* const srcSlice[],
									tmp_frame->linesize,    //const int srcStride[],
									0,                       //int srcSliceY,
									tmp_frame->height,      //int srcSliceH,
									pYUV420Frame->data,      //uint8_t* const dst[],
									pYUV420Frame->linesize); //const int dstStride[]);
								if (sts == tmp_frame->height) {
									bsucc = true;
								}
								else {
									int i = 0;
								}
							}
							else {
								pYUV420Frame = tmp_frame;
								bsucc = true;
							}
						}
					}
					else {
						pYUV420Frame = frame;
						bsucc = true;
					}

					if (bsucc) {
						int yuv_len = av_image_get_buffer_size((AVPixelFormat)(pYUV420Frame->format), pYUV420Frame->width,
							pYUV420Frame->height, 1);
						int bufidx = yuv_len;
						if (yuv_vec.size() != yuv_len) {
							yuv_vec.resize(yuv_len);
						}
						av_image_copy_to_buffer(&(yuv_vec[0]), yuv_len,
							(const uint8_t* const*)pYUV420Frame->data,
							(const int*)pYUV420Frame->linesize, (AVPixelFormat)pYUV420Frame->format,
							pYUV420Frame->width, pYUV420Frame->height, 1);

                        eVideoRender::PIXEL_FORMAT fmt = eVideoRender::PIXEL_FORMAT::RENDER_UNKNOW;
						if (pYUV420Frame->format == AV_PIX_FMT_YUV420P) {
                            fmt = eVideoRender::PIXEL_FORMAT::RENDER_YUV420P;
						}
						else if (pYUV420Frame->format == AV_PIX_FMT_NV12) {
                            fmt = eVideoRender::PIXEL_FORMAT::RENDER_NV12;
						}
                        if (fmt != eVideoRender::PIXEL_FORMAT::RENDER_UNKNOW) {
							if (eAVRender::g_inst->_gl_render) {
								eAVRender::g_inst->_gl_render->filldata(yuv_vec, bufidx, pYUV420Frame->width, pYUV420Frame->height, fmt);
							}
							else {
								gTrans->SendAVData("video_data_handle", yuv_vec, bufidx, pYUV420Frame->width, pYUV420Frame->height, (int)fmt);
							}
						}
					}
				}

				av_frame_unref(frame);
			}
		}
		else if (packet->stream_index == audio_stream_index) {
			ret = avcodec_send_packet(audio_dec_ctx, packet);
			if (ret < 0) {
				av_log(NULL, AV_LOG_ERROR, "Error while sending a packet to the decoder\n");
				break;
			}

			while (ret >= 0) {
				ret = avcodec_receive_frame(audio_dec_ctx, frame);
				if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
					break;
				}
				else if (ret < 0) {
					av_log(NULL, AV_LOG_ERROR, "Error while receiving a frame from the decoder\n");
					goto end;
				}

				if (ret >= 0) {
					int data_size = av_get_bytes_per_sample(audio_dec_ctx->sample_fmt);
					if (data_size >= 0) {
						int dst_nb_samples = av_rescale_rnd(swr_get_delay(g_audio_dec_ctx->convert_audio_ctx, frame->sample_rate) + frame->nb_samples,
							AUDIO_PLAY_SAMPLE,//得到变换后采用点数
							frame->sample_rate, AV_ROUND_INF);
						//audio_out_buffer
						uint8_t* output;
						int err = av_samples_alloc(&output, NULL, 2, dst_nb_samples,
							AUDIO_DATA_TYPE, 0);
						int len = swr_convert(g_audio_dec_ctx->convert_audio_ctx, &output, dst_nb_samples, (const uint8_t**)frame->data, frame->nb_samples);   //变换器启动
						//audio_buf = *audio_out_buffer;  //获得输出数据
						int audio_len = len * av_get_bytes_per_sample(AUDIO_DATA_TYPE) * 2;	//获取输出音频数据大小
						eAVRender::g_inst->writeAudioRingBuf(output, audio_len);
						av_freep(&output);
					}
					av_frame_unref(frame);
				}
			}
		}
		av_packet_unref(packet);
	}
end:
	avcodec_free_context(&video_dec_ctx);
	avcodec_free_context(&audio_dec_ctx);
	avformat_close_input(&fmt_ctx);
	av_frame_free(&frame);
	av_frame_free(&filt_frame);
	av_packet_free(&packet);

	if (ret < 0 && ret != AVERROR_EOF) {
		char sz[128] = {0};
		av_make_error_string(sz, 128, ret);
		fprintf(stderr, "Error occurred: %s\n", sz);
    }
	b_play_done = true;
    printf("playfile done\n");
	return 0;
}
