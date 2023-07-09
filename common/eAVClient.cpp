#include "eAVClient.h"
//#include <ws2tcpip.h>
//#include <windows.h>
//#include <io.h>
#include <fcntl.h>
#include <event2/event.h>
#include <thread>
#include <signal.h>
#include "../base/misctool.h"
#include "stdio.h"
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifndef S_ISDIR
#define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
#endif

#define _BUFFER_SIZE 8192

struct event* eAVClient::s_signal_int = nullptr;
struct event_base* eAVClient::s_base = nullptr;
bool eAVClient::s_bInit = false;
long eAVClient::s_global_id = 0;
std::map<long, eAVClient*> eAVClient::_avclients;
std::mutex eAVClient::s_lock;
neosmart::neosmart_event_t eAVClient::s_hReadyEvent = nullptr;


/**
* 设置socket为非阻塞模式。
*/
int setnonblock(int fd)
{
#ifdef _WIN32
	unsigned long ul = 1;
	return  ioctlsocket(fd, FIONBIO, &ul);
#else
	int flags;

	flags = fcntl(fd, F_GETFL);
	if (flags < 0)
		return flags;
	flags |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags) < 0)
		return -1;

	return 0;
#endif

}


void eAVClient::signal_cb(evutil_socket_t fd, short event, void* arg) {
	printf("signal_cb.....\n");
	event_base_loopexit(eAVClient::s_base, nullptr);
}

eAVClient::eAVClient() {
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_port = htons(0);
	std::unique_lock<std::mutex> lock(s_lock);
	_session_id = ++s_global_id;
	_avclients.insert(std::make_pair(_session_id, this));
}

eAVClient::~eAVClient() {

	//WSACleanup();
	RemoveClient(_session_id);
}

void eAVClient::Quit() {
	event_active(eAVClient::s_signal_int, SIGINT, 0);
}

void eAVClient::RunGlobalThread() {
	int err = 0;
#ifdef _WIN32
	WSADATA wsa_data;
	WORD wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsa_data);
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* Winsock DLL.                                  */
		printf("WSAStartup failed with error: %d\n", err);
		return;
	}
#endif
    setsigmask();
    evthread_use_pthreads();
	struct event_config* cfg = event_config_new();
	//event_config_set_flag(cfg, EVENT_BASE_FLAG_STARTUP_IOCP);
	//根据CPU实际数量配置libEvent的CPU数
    //SYSTEM_INFO si;
    //GetSystemInfo(&si);
	//event_config_set_num_cpus_hint(cfg, si.dwNumberOfProcessors);

	eAVClient::s_base = event_base_new_with_config(cfg);
	event_config_free(cfg);

	evthread_make_base_notifiable(eAVClient::s_base);


	eAVClient::s_signal_int = evsignal_new(eAVClient::s_base, SIGINT, signal_cb, nullptr);
	event_add(eAVClient::s_signal_int, NULL);
    //printf("RunGlobalThread threadid: %d\n", GetCurrentThreadId());
    neosmart::SetEvent(s_hReadyEvent);
	err = event_base_dispatch(eAVClient::s_base);
	printf("RunGlobalThread exit %d !!!!!! \n", err);
	evsignal_del(eAVClient::s_signal_int);
	eAVClient::s_signal_int = nullptr;
    //CloseHandle(s_hReadyEvent);
    neosmart::DestroyEvent(s_hReadyEvent);
	s_hReadyEvent = nullptr;
	//event_base_loop(eAVClient::s_base, 0);
	//printf("RunGlobalThread exit \n");
}

void eAVClient::Init() {
	if (!eAVClient::s_bInit) {
        //s_hReadyEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
        s_hReadyEvent = neosmart::CreateEvent();
		std::thread thd(&eAVClient::RunGlobalThread);
		thd.detach();
		eAVClient::s_bInit = true;
	}
}

void eAVClient::Loop_run(const char* szAdds, const short int port) {
	//printf("Loop_run threadid: %d\n", GetCurrentThreadId());
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_port = htons(port);
#ifdef WIN32  
	//serverAddr.sin_addr.S_un.S_addr = inet_addr(szAdds);
	inet_pton(AF_INET, szAdds, &_serverAddr.sin_addr);
#else  
    _serverAddr.sin_addr.s_addr = inet_addr(szAdds);
#endif  
	memset(_serverAddr.sin_zero, 0x00, sizeof(_serverAddr.sin_zero));

	do_real_connect();
}

void eAVClient::read_cb(struct bufferevent* bev, void* arg) {
	long session = (long)arg;
	eAVClient* cli = GetClient(session);
	if (cli == nullptr) {
		return;
	}
	int n = 0;
	evutil_socket_t fd = bufferevent_getfd(bev);

	static unsigned char recvBuf[_BUFFER_SIZE + 1];
	while (n = bufferevent_read(bev, recvBuf, _BUFFER_SIZE), n > 0)
	{
		//printf("read_cb\n");
		if (cli->_recvCb){
			cli->_recvCb(recvBuf, n, cli->_recv_ctx);
		}
	}
}

void eAVClient::event_cb(struct bufferevent* bev, short events, void* arg) {
	long session = (long)arg;
	eAVClient* cli = GetClient(session);
	if (cli == nullptr) {
		return;
	}
	printf("event_cb events:%0x\n", events);
	if (events & BEV_EVENT_EOF) {
		cli->_status = RetmoteStatus::DISCONNECT;
		printf("Connection closed (eof).\n");
	}
	else if (events & BEV_EVENT_ERROR) {
		cli->_status = RetmoteStatus::DISCONNECT;
		printf("Got an error on the connection IOCP? \n");
		//return;
	}
	else if (events & BEV_EVENT_TIMEOUT) {
		cli->_status = RetmoteStatus::DISCONNECT;
		printf("Connection time out.\n");
	}
	else if (events & BEV_EVENT_CONNECTED)
	{
		cli->_status = RetmoteStatus::CONNECTED;
		if (cli->_connectCb) {
			cli->_connectCb(cli->_connect_ctx);
		}
		printf("CONNECTED ok\n");
		return;
	}
	else {
		return;
	}
	/* None of the other events can happen here, since we haven't enabled
	 * timeouts */
	//closesocket(cli->_sock);
	if (cli->_bev) {
		bufferevent_setcb(cli->_bev, nullptr, nullptr, nullptr, nullptr);
		bufferevent_disable(cli->_bev, EV_READ | EV_PERSIST | EV_CLOSED);
		bufferevent_free(cli->_bev);
	}
	cli->_bev = nullptr;
	if (cli->_bDone) {
		//event_base_loopexit(eAVClient::s_base, nullptr);
		cli->call_error_cb();
		delete cli;
	}
	else {
		cli->delay_reconn();
	}
	
}

bool eAVClient::Connect(const char* szAdd, const short int port) {
	bool ret = false;
	EVENT_CTX* ctx = new EVENT_CTX;
	ctx->port = port;
    strcpy(ctx->szuri, szAdd);
	struct event* ev = evtimer_new(eAVClient::s_base, do_conn_cb, (void*)ctx);
	ctx->ev = ev;
	ctx->self = this;
	timeval tv;
	tv.tv_sec = 0; //try delay 0 sec
	tv.tv_usec = 0;
	if (ev) {
		if (evtimer_add(ev, &tv) == 0) {
			ret = true;
		}
		else {
			event_free(ev);
		}
	}
	return ret;
}

bool eAVClient::Disconnect() {
	bool ret = false;
	EVENT_CTX* ctx = new EVENT_CTX;
	struct event* ev = evtimer_new(eAVClient::s_base, do_close_cb, (void*)ctx);
	ctx->ev = ev;
	ctx->avclient_id = _session_id;
	timeval tv;
	tv.tv_sec = 0; //try delay 0 sec
	tv.tv_usec = 0;
	if (ev) {
		if (evtimer_add(ev, &tv) == 0) {
			ret = true;
		}
		else {
			event_free(ev);
		}
	}
	return ret;
}

bool eAVClient::SendData(const unsigned char* data, const int len) {
	if (_status != RetmoteStatus::CONNECTED) {
		printf("not connect dont send!! %d\n", _dummyid);
		return false;
	}
	EVENT_CTX* ctx = new EVENT_CTX;
	ctx->self = this;
	ctx->data = new unsigned char[len+4+1];
	memset(ctx->data, 0, len + 4+1);
	ctx->len = len+4;
	//int newlen = ((len & 0x00ff) << 16) | ((len & 0xff00) >> 16 );
	unsigned int newlen = htonl(len);
	//printf("new 0x%08x, old 0x%08x\n", newlen, len);
	memcpy(ctx->data, &newlen, 4);
	memcpy(ctx->data+4, data, len);
	struct event* ev = evtimer_new(eAVClient::s_base, do_send_cb, (void*)ctx);
	ctx->ev = ev;
	bool ret = false;
	timeval tv;
	tv.tv_sec = 0; //try delay 0 sec
	tv.tv_usec = 0;
	if (ev) {
		if (evtimer_add(ev, &tv) == 0) {
			ret = true;
		}
		else {
			delete[] ctx->data;
			delete ctx;
			event_free(ev);
		}
	}

	return ret;
}

void eAVClient::do_conn_cb(int fd, short events, void* arg) {
	EVENT_CTX* ctx = (EVENT_CTX*)(arg);
	event_free(ctx->ev);
	if (ctx->self->_status != RetmoteStatus::CONNECTED) {
		ctx->self->Loop_run(ctx->szuri, ctx->port);
	}
	else {
		printf("cliend has connected\n");
	}
	delete ctx;
}

void eAVClient::do_close_cb(int fd, short events, void* arg) {
	EVENT_CTX* ctx = (EVENT_CTX*)(arg);
	eAVClient* self = GetClient(ctx->avclient_id);
	event_free(ctx->ev);
	if (self == nullptr) {
		return;
	}
	self->_bDone = true;
	if (self->_bev) {
		bufferevent_trigger_event(self->_bev, BEV_EVENT_ERROR, 0);
		//ctx->self->_status = RetmoteStatus::DISCONNECT;
		//closesocket(ctx->self->_sock);
		//bufferevent_free(ctx->self->_bev);
		//ctx->self->_bev = nullptr;
		//ctx->self->call_error_cb();
	}
	delete ctx;
}

void eAVClient::do_send_cb(int fd, short events, void* arg) {
	static constexpr int MAX_SEND_TIMES = 4096;
	EVENT_CTX* ctx = (EVENT_CTX*)arg;
	event_free(ctx->ev);
	int remind = ctx->len;
	int begin = 0;
	int userage = 0;
	while (ctx->self->_status == RetmoteStatus::CONNECTED) {
		userage = remind <= MAX_SEND_TIMES ? remind : MAX_SEND_TIMES;
		if (userage <= 0) {
			break;
		}
		if (bufferevent_write(ctx->self->_bev, ctx->data + begin, userage) < 0 ) {
			break;
		}
		begin += userage;
		remind -= userage;
	}
    std::string strts;// = GetCurrentTimeStamp(1);
	//long long ts = utime_ms();
	//printf("send data ts(%lld): %s\n", ts, (char*)(ctx->data+4));
	printf("send data ts(%s): %s\n", strts.c_str(), (char*)(ctx->data + 4));
	delete[] ctx->data;
	delete ctx;
}

void eAVClient::do_reconn_cb(int fd, short events, void* arg) {
	EVENT_CTX* ctx = (EVENT_CTX*)(arg);
	event_free(ctx->ev);
	
	ctx->self->help_reconn();
	delete ctx;
}

void eAVClient::delay_reconn() {
	srand(time(0));
	EVENT_CTX* ctx = new EVENT_CTX;
	struct event* ev = evtimer_new(eAVClient::s_base, do_reconn_cb, (void*)ctx);
	ctx->ev = ev;
	ctx->self = this;
	timeval tv;
	tv.tv_sec = rand() % 6 + 2; //try delay 0 sec
	tv.tv_usec = 0;
	if (ev) {
		if (evtimer_add(ev, &tv) == 0) {

		}
		else {
			event_free(ev);
		}
	}
}

void eAVClient::help_reconn() {
	printf("help_reconn \n");	
	do_real_connect();
	
}

void eAVClient::call_error_cb() {
	if (_errorCb) {
		printf("call_error_cb %d\n", (long)_error_ctx);
		_errorCb(ERR_CODE::ERROR_CODE_ERROR, _error_ctx);
	}
}

void eAVClient::do_real_connect() {
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int err = -100;
	int on = 1;
#ifdef WIN32  
	setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (const char*)&on, sizeof(int));
	setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&on, sizeof(int));
#else  
    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (void*)&on, sizeof(on));
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&on, sizeof(int));
#endif
	if (evutil_make_socket_nonblocking(fd) == 0) {
		printf("sock no block\n");
	}
	_bev = bufferevent_socket_new(eAVClient::s_base, fd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);
	bufferevent_setcb(_bev, read_cb, nullptr, event_cb, (void*)(this->_session_id));
	bufferevent_enable(_bev, EV_READ | EV_PERSIST | EV_CLOSED);
	_status = RetmoteStatus::UNNKOW;
	if (bufferevent_socket_connect(_bev, (struct sockaddr*)&_serverAddr, sizeof(_serverAddr)) == 0) {
	//if(connect(_sock, (struct sockaddr*)&_serverAddr, sizeof(_serverAddr)) != -1){
		//succ
		//evutil_make_socket_nonblocking(fd);
	}
	else {
		//connect error
		printf("reconn fail\n");
		bufferevent_free(_bev);
		_bev = nullptr;
		delay_reconn();
	}
}

void eAVClient::RemoveClient(long id) {
	std::unique_lock<std::mutex> lock(s_lock);
	auto got = _avclients.find(id);
	if (got != _avclients.end()) {
		_avclients.erase(got);
	}
}

eAVClient* eAVClient::GetClient(long id) {
	std::unique_lock<std::mutex> lock(s_lock);
	eAVClient* client = nullptr;
	auto got = _avclients.find(id);
	if (got != _avclients.end()) {
		client = got->second;
	}
	return client;
}
