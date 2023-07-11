#include "misctool.h"
#include <signal.h>
#include "base/uuid/uuid.h"
#include <boost/crc.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/functional/hash.hpp>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/timeb.h>
//#include <qwindow.h>
#include <sys/types.h>
#include <unistd.h>

bool IsWindow(uint64_t id){
    //QWindow* win = QWindow::fromWinId(id);
    //return win != nullptr;
    return true;
}

//#include "elog.h"

/*bool testTcpPortUsed(unsigned short usPort) {
    bool bUsed = false;
    int s = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(usPort);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int ret = bind(s, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1 && errno == EADDRINUSE	){
        bUsed = true;
    }
    close(s);
    return bUsed;
}*/

std::string CreateGuid()
{
    uuid_t uu;
    char str[GUID_KEY_LEN];
    uuid_generate( uu );
    uuid_unparse(uu, str);
    return std::string(str);
}


void CRC64(const std::string &inputStr)
{
   boost::crc_optimal<64, 0x42F0E1EBA9EA3693,
                      0xffffffffffffffff, 0xffffffffffffffff,
                      false, false> crc;
}

std::string generate_guid(){
    boost::uuids::uuid uid = boost::uuids::random_generator()();
    return boost::uuids::to_string(uid);
}

int getnow_ts(struct timespec &now_ts){
    int err = 0;
#if !defined _POSIX_MONOTONIC_CLOCK || _POSIX_MONOTONIC_CLOCK < 0
    clock_gettime(CLOCK_REALTIME, &spec);
#elif _POSIX_MONOTONIC_CLOCK > 0
    clock_gettime(CLOCK_MONOTONIC, &spec);
#else
    err = clock_gettime(CLOCK_MONOTONIC, &now_ts);
    if (err)
        err = clock_gettime(CLOCK_REALTIME, &now_ts);
#endif
    return err;
}

int get_now_utc_ts(struct timespec &now_ts){
    int err = 0;
    err = clock_gettime(CLOCK_REALTIME, &now_ts);
    return err;
}

int utc_time_str(std::string& strTime){
    time_t cur_time = time(nullptr);
    if( cur_time < 0 ){
        return -1;
    }
    struct tm utc_tm;
    if( nullptr == gmtime_r( &cur_time, &utc_tm ) ){
        return -1;
    }
    //strTime = asctime(utc_tm);
    return 0;
}

long getNowTickMs(){
    long now_ms = 0;
    struct timespec ts;
    if(getnow_ts(ts)==0){
        now_ms = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    }
    return now_ms;
}

long getUtcTickSec(){
    struct timespec now_ts;
    if(clock_gettime(CLOCK_REALTIME, &now_ts) != 0){
        return -1;
    }
    return now_ts.tv_sec;
}

unsigned long getUtcTickus(){
    unsigned long  now_us = 0;
    struct timespec ts;
    if(get_now_utc_ts(ts)==0){
        now_us = ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
    }
    return now_us;
}

unsigned long getUtcTickms(){
    unsigned long  now_ms = 0;
    struct timespec ts;
    if(get_now_utc_ts(ts)==0){
        now_ms = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    }
    return now_ms;
}

unsigned int hash_str(std::string &str){
    boost::hash<std::string> string_hash;
    unsigned int hash_value = string_hash(str);
    return hash_value;
}


//默认读写一个关闭的socket会触发sigpipe信号 该信号的默认操作是关闭进程 有时候这明显是我们不想要的
//所以此时我们需要重新设置sigpipe的信号回调操作函数  比如忽略操作等  使得我们可以防止调用它的默认操作 
//信号的处理是异步操作 也就是说 在这一条语句以后继续往下执行中如果碰到信号依旧会调用信号的回调处理函数
//处理sigpipe信号
void handle_for_sigpipe(){
    struct sigaction sa; //信号处理结构体
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = SIG_IGN;//设置信号的处理回调函数 这个SIG_IGN宏代表的操作就是忽略该信号 
    sa.sa_flags = 0;
    if(sigaction(SIGPIPE, &sa, NULL) == 0){//将信号和信号的处理结构体绑定
        //printLogMsg(eLOG_LEVEL_WARN, "global", "SIGPIPE ignore");
	}
}


int setsigmask(){
    sigset_t mask;
    sigset_t orig_mask;
    sigemptyset(&mask);

    sigaddset(&mask, SIGPIPE);
    //pthread_sigmask(SIG_BLOCK, &mask, &orig_mask);

    if (sigprocmask(SIG_BLOCK, &mask, &orig_mask) < 0) {
        //printLogMsg(eLOG_LEVEL_WARN, "eTcpSrvLayer", "set sigpipe ignore");
        return 1;
    }
    return 0;
}

static void Char2Hex(const unsigned char ch, char* szHex)
{
	unsigned char byte[2];
	byte[0] = ch / 16;
	byte[1] = ch % 16;
	for (int i = 0; i < 2; i++)
	{
		if (byte[i] >= 0 && byte[i] <= 9)
			szHex[i] = '0' + byte[i];
		else
			szHex[i] = 'A' + byte[i] - 10;
	}
	szHex[2] = 0;
}


static void CharStr2HexStr(const unsigned char* pucCharStr, char* pszHexStr, int iSize)
{
	int i;
	char szHex[3];
	pszHexStr[0] = 0;
	for (i = 0; i < iSize; i++)
	{
		Char2Hex(pucCharStr[i], szHex);
		strcat(pszHexStr, szHex);
	}
}

std::string GetStringMd5(const char* pszData)
{
	unsigned char md5[16] = { 0 };
	MD5((const unsigned char*)pszData, strlen(pszData), md5);
	char strMd5[34] = {};
	CharStr2HexStr(md5, strMd5, 16);

	return strMd5;
}

std::string GetStringSha256(const char* pszData){
    unsigned char szsha256[34] = { 0 };
	SHA256((const unsigned char*)pszData, strlen(pszData), szsha256);
	char strSha256[80] = {0};
	CharStr2HexStr(szsha256, strSha256, 32);

	return strSha256;
}

std::vector<std::string> stringSplit(const std::string& str, char delim) {
    std::string s;
    s.append(1, delim);
    std::regex reg(s);
    std::vector<std::string> elems(std::sregex_token_iterator(str.begin(), str.end(), reg, -1),
                                   std::sregex_token_iterator());
    return elems;
}

#ifdef HAVE_SCHED_H
#include <sched.h>
#endif

void setCurrentThreadHighPriority(bool value) {
  // Start out with a standard, low-priority setup for the sched params.
  struct sched_param sp;
  bzero((void*)&sp, sizeof(sp));
  int policy = SCHED_OTHER;

  // If desired, set up high-priority sched params structure.
  if (value) {
    // FIFO scheduler, ranked above default SCHED_OTHER queue
    policy = SCHED_FIFO;
    // The priority only compares us to other SCHED_FIFO threads, so we
    // just pick a random priority halfway between min & max.
    const int priority = (sched_get_priority_max(policy) + sched_get_priority_min(policy)) / 2;

    sp.sched_priority = priority;
  }

  // Actually set the sched params for the current thread.
  if (0 == pthread_setschedparam(pthread_self(), policy, &sp)) {
    printf("IO Thread #%ld using high-priority scheduler!", pthread_self());
  }
}

//time_stamp_type - 需要获取的时间戳的级别，0表示秒级时间戳，1表示毫秒级时间戳，2表示微秒级时间戳，3表示纳秒级时间戳
std::string GetCurrentTimeStamp(int time_stamp_type /*= 0*/){
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

	std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
	std::tm* now_tm = std::localtime(&now_time_t);

	char buffer[128] = {0};
	strftime(buffer, sizeof(buffer), "%F %T", now_tm);

	std::ostringstream ss;
	ss.fill('0');

	std::chrono::milliseconds ms;
	std::chrono::microseconds cs;
	std::chrono::nanoseconds ns;
	
	switch (time_stamp_type)
	{
	case 0:
		ss << buffer;
		break;
	case 1:
		ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
		ss << buffer << "." << ms.count();
		break;
	case 2:
		ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
		cs = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
		ss << buffer << "." << ms.count() << "." << cs.count() % 1000;
		break;
	case 3:
		ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
		cs = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
		ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()) % 1000000000;
		ss << buffer << "." << ms.count() << "." << cs.count() % 1000 << "." << ns.count() % 1000;
		break;
	default:
		ss << buffer;
		break;
	}

	return ss.str();
}

void getNowTimeFmt(std::string& out){
    //auto now = std::chrono::system_clock::now();
    //auto tm_time = spdlog::details::os::localtime(std::chrono::system_clock::to_time_t(now));
    //char date_buf[64] = {0};
    //std::strftime(date_buf, sizeof(date_buf), "%Y-%m-%d %H:%M:%S.%e", &tm_time);
    //out = date_buf;
    out = GetCurrentTimeStamp(1);
}

bool testTcpPortUsed(unsigned short usPort) {
	bool bUsed = false;
	int s = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(usPort);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	int ret = bind(s, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1 && errno == EADDRINUSE){
        bUsed = true;
    }
	close(s);
	return bUsed;
}

std::string GetExeDirectory(){
    long pid = getpid();
    char str[512] = {0};
    char buf[512] = {0};
    snprintf(str, sizeof(str), "/proc/%ld/exe", pid);
    readlink(str, buf, sizeof(str));
    std::string dir(buf);
    auto found = dir.rfind('/');
    if (found != std::string::npos) {
        dir.erase(found);
    }
    return dir;
}
