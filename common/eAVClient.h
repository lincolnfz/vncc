#pragma once
#include <functional>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/thread.h>
#include <map>
#include <mutex>
#include <base/event/pevents.h>

/*
* 连接音视频服务数据
*/

class eAVClient
{
public:
	enum class RetmoteStatus {
		UNNKOW = -1,
		DISCONNECT = 0,
		CONNECTED = 1,
	};
	struct EVENT_CTX {
		struct event* ev = nullptr;
		unsigned char* data = nullptr;
		short int len = 0;
		eAVClient* self = nullptr;
		long avclient_id = 0;
		char szuri[261] = {0};
		short int port = 0;
	};

	enum class ERR_CODE {
		ERROR_CODE_BEGIN,
		ERROR_CODE_EOF,
		ERROR_CODE_ERROR,
		ERROR_CODE_TIMEOUT,

		//////////////////////////////////////////////////////////////////////////
		//end
		ERROR_CODE_END,
	};
	typedef std::function<void(void *)> ConnectCb;
	typedef std::function<void(ERR_CODE, void*)> ErrorCb;
	typedef std::function<void(const unsigned char*, const int, void*)> CliRecvDataCb;
	static void Init();
	static void Quit();
	bool Connect(const char* szAdd, const short int port);
	bool Disconnect();
	bool SendData(const unsigned char* data, const int len);
	void SetConnectCB(ConnectCb cb, void* ctx) {
		_connectCb = cb;
		_connect_ctx = ctx;
	};
	void SetErrorCB(ErrorCb cb, void* ctx) {
		_errorCb = cb;
		_error_ctx = ctx;
	};
	void SetRecvCB(CliRecvDataCb cb, void* ctx) {
		_recvCb = cb;
		_recv_ctx = ctx;
	};
	virtual ~eAVClient();
	eAVClient();
	//void RemoteConnect()
	void SetDummyID(long id) {
		_dummyid = id;
	}

	void SetToken(std::string& token) {
		if (!token.empty()) {
			_token = token;
		}
	}

	std::string Token() {
		return _token;
	}

	static struct event_base* s_base;
    static neosmart::neosmart_event_t s_hReadyEvent;
protected:
	static void RunGlobalThread();
	static void signal_cb(evutil_socket_t fd, short event, void* arg);
	static void read_cb(struct bufferevent* bev, void* arg);
	static void event_cb(struct bufferevent* bev, short events, void* arg);
	static void do_conn_cb(int fd, short events, void* arg);
	static void do_close_cb(int fd, short events, void* arg);
	static void do_send_cb(int fd, short events, void* arg);
	static void do_reconn_cb(int fd, short events, void* arg);
	static void RemoveClient(long id);
	static eAVClient* GetClient(long key);
	void Loop_run(const char* szAdd, const short int port);
	void delay_reconn();
	void help_reconn();
	void call_error_cb();
	void do_real_connect();

	static struct event* s_signal_int;
	struct bufferevent* _bev = nullptr;


	ConnectCb _connectCb;
	ErrorCb _errorCb;
	CliRecvDataCb _recvCb;
	RetmoteStatus _status = RetmoteStatus::UNNKOW;
	static bool s_bInit;
	bool _bDone = false;
	struct sockaddr_in _serverAddr;
	void* _connect_ctx = nullptr;
	void* _error_ctx = nullptr;
	void* _recv_ctx = nullptr;
	long _dummyid = 0;
	static std::map<long, eAVClient*> _avclients;
	static long s_global_id;
	static std::mutex s_lock;
	long _session_id = 0;
	std::string _token;
};

