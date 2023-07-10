#ifndef eavrender_h
#define eavrender_h

// base header
#include "base/base.h"
#include "common/evideorender.h"
#include "common/defThread.h"

/*���λ�����������*/
typedef struct
{
	unsigned char* buf;    /*���λ�����        */
	unsigned int size;     /*���λ�����        */
	unsigned int front;    /*ͷָ��            */
	unsigned int rear;     /*βָ��            */
}ring_buf_t;

/*-------------------------�ⲿ�ӿ�����----------------------------*/
/*-------------------------ring_buf_t ���Ⱦ���ȡ 2^n  -------------*/
int ring_buf_create(ring_buf_t* r, unsigned char* buf, unsigned int size);

int ring_buf_realloc(ring_buf_t* old_r, ring_buf_t* new_r, unsigned char* new_buf, unsigned int new_size);

void ring_buf_clr(ring_buf_t* r);

unsigned int ring_buf_len(ring_buf_t* r);

unsigned int ring_buf_remain(ring_buf_t* r);

unsigned int ring_buf_put(ring_buf_t* r, unsigned char* buf, unsigned int len);

unsigned int ring_buf_get(ring_buf_t* r, unsigned char* buf, unsigned int len);

/*
* ���ӷ�������ȡ������
*/

class eAVRender : public nbase::Thread , public nbase::SupportWeakCallback
{
public:
    eAVRender(enum ThreadId thread_id, const char* name, const char* orderid)
        : thread_id_(thread_id) {
		_orderid = orderid;
	}

    void SetExtInfo(const char* token, const char* guid,
                    const void* glwnd, const int init_wdith, const int init_height);

	virtual ~eAVRender(void) {}

	static eAVRender* g_inst;

	void writeAudioRingBuf(unsigned char* buf, int len);

	void readAudioRingBuf(unsigned char* outbuf, int& outlen, int maxlen);

	unsigned int audio_buf_len();

	void savevideo(unsigned char* buf, int len);

	void saveaudio(unsigned char* buf, int len);

	void savelog(const char* log);

	void setParm(const char* av_addr, const char* ws_addr, const char* stun_addr, const int connect_type, const int data_type);

	void helpReconnect();

	void DecodeVideo(std::vector<unsigned char> data);

	void StopVideo();

	void triggerStraamThread();

	int PlayFile();

	void SetInitWithConnect(bool v) {
		_b_init_with_connectStream = v;
	};

	void loop_check_render();

    //HANDLE _fp = NULL;

    std::shared_ptr<eVideoRender> _gl_render;

	bool _report_resolution = false;
private:

	/**
	* �麯������ʼ���߳�
	* @return void	�޷���ֵ
	*/
	virtual void Init() override;

	/**
	* �麯�����߳��˳�ʱ����һЩ������
	* @return void	�޷���ֵ
	*/
	virtual void Cleanup() override;

	int openSDL();

	static void sdlAudioCallBackFunc(void* userdata, unsigned char* stream, int len);

	bool openAucioPlayClient();

    static void msAudioCBFunc(unsigned char*, unsigned int, unsigned int, void*);

    static unsigned int streamConnect(void*);
private:
	//std::shared_ptr<eGLRender> _gl_render;
    enum ThreadId thread_id_;
    void* _hmod = nullptr;
	ring_buf_t* _audio_ring_buf = nullptr;
	unsigned char* _audio_buf = nullptr;
    //std::mutex _audio_mutex;
    std::mutex _audio_cs;
	std::string _str_av_addr, _str_ws_addr, _str_stun_addr;
	std::string _orderid, _token, _guid;
	int _connect_type = 0;
	int _data_type = 2;
	FILE* _video_fp = nullptr;
	FILE* _audio_fp = nullptr;
	FILE* _log_fp = nullptr;
    unsigned long _bytePreSecLen = 0; //windows ����ÿ����Ҫ���ֽ���
    bool _b_init_with_connectStream = false;
    int64_t _gl_hWnd = 0;
    int _gl_width = 0;
    int _gl_height = 0;
};

#endif
