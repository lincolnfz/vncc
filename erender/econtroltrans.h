#ifndef ECONTROLTRANS_H
#define ECONTROLTRANS_H
#include "base/base.h"
#include "common/eAVClient.h"
#include "common/defThread.h"

using RecvCALLBACK = std::function<void(const char*)>;
using FailCALLBACK = std::function<void(void)>;

#define CMD_CONN 0x1
#define RT_CONN 0x2
#define AV_CONN 0x4
#define FULL_CONN (CMD_CONN|RT_CONN|AV_CONN)

using DATA_PACKAGE = std::vector<unsigned char>;
class eAVRender;

class RenderHelpThrad : public nbase::Thread {
public:
    RenderHelpThrad(){};
    virtual ~RenderHelpThrad(){};

    virtual void Init() override {

    };

    /**
    * 虚函数，主线程退出时，做一些清理工作
    * @return void	无返回值
    */
    virtual void Cleanup() override {

    };
};


class eControlTrans : public nbase::SupportWeakCallback, public nbase::Thread
{
public:
    enum class RemoteStatus {
        DISCONNECT = 0,
        CONNECTED = 1,
    };
    eControlTrans(enum ThreadId thread_id, const char* name, const char* orderid, const char* av_addr, const char* remote_addr, const char* cmd_addr,
        const char* ws_addr, const char* stun_addr, int connect_type, int data_type,
        unsigned short rpc_port, unsigned int keyid)
        : thread_id_(thread_id) {
        _orderid = orderid;
        _remote_url = remote_addr;
        _cmd_url = cmd_addr;
        _av_url = av_addr;
        _ws_url = ws_addr;
        _stun_url = stun_addr;
        _connect_type = connect_type;
        _rpc_port = rpc_port;
        _keyid = keyid;
        _data_type = data_type;
    }

    virtual ~eControlTrans(void) {
        //if (_hExitEvent) {
        //    CloseHandle(_hExitEvent);
        //}
    }

    unsigned short Port() const { return _port; };

    void SendAVData(const char* fn_name, std::vector<unsigned char>& data, int size, int width, int height, int pixfmt);

    void goDisconnect(const char* fn_name, const int disconnect_type);

    void SendCollectData(const std::string datatype, const std::string data, const bool bhighfeq);

    void SendResolution(int width, int height);

    void UpdateRatio(int l, int s);

    void SendRotate(int rotate, int width, int height);

    void SetExtInfo(const char* token, const char* guid, const char* lang) {
        _token = token;
        _guid = guid;
        _lang = lang;
    }

    void SetConnState(short type);
    void RemoveConnState(short type);

    void SendFrameActivate();
    void SetWindow(uint64_t winid, int w, int h);

protected:
    bool SendTcpData(const char* name, const unsigned char* data, const int len, RecvCALLBACK, FailCALLBACK);
    void RouteCmd(const char* szjson);

private:
    /**
    * 虚函数，初始化线程
    * @return void	无返回值
    */
    virtual void Init() override;

    /**
    * 虚函数，线程退出时，做一些清理工作
    * @return void	无返回值
    */
    virtual void Cleanup() override;

    static void DoOp(std::string json);
    static void close_self();
    static void TransMouse(std::string strjson);
    static void SendEventMsg(std::string strjson, int type);
    static void Roate(float angle);
    static void Bitrate(int bitrate, int bauto);
    static void SetFps(int fps);
    static void Preview(bool);
    static void ScreenCap(long user_order_id, std::string out_filepath);
    static void DoAlive();
    static void GoDebug();
    void SafeSendRemoteMsg(std::string msg);
    void SafeSendCmdMsg(std::string msg);
    void SaveRoate(float angle);
    void RemoteControlRun();
    void HelpRemoteRun();
    void HelpErrCB();
    void RecvData(std::vector<unsigned char>& data);
    void SendHeart();
    void SendClearApps();
    void SendSetProp(std::string szMode);
    void SendAmBroadcast();
    void HelpPreview();
    void help_ScreenCap(long user_order_id, std::string out_filepath);

    void DecodeBase64_2_File(const char* pbase64);
    //切换语言
    void SendSwitchLang();
    void QueryDeviceInfo();

    void RemoveCB(unsigned int val);
    void StartRender();
    static void RemoteConnectCB(void* user_arg);
    static void recvRemoteDataCB(const unsigned char*, const int, void* user_arg);
    static void ErrCB( eAVClient::ERR_CODE , void* user_arg);
    static void rt_RemoteConnectCB(void* user_arg);
    static void rt_ErrCB(eAVClient::ERR_CODE, void* user_arg);
    static void touch_move();
    static void touch_down();
    static void touch_up();

    void CheckAlive();

private:
    enum ThreadId thread_id_;
    //std::unique_ptr<eAVRender> _render;
    std::shared_ptr<eAVRender> _render;
    std::shared_ptr<RenderHelpThrad> _glrenderThd;
    std::string _remote_url, _cmd_url, _av_url, _ws_url, _stun_url, _orderid;
    unsigned short _port = 19055;
    unsigned short _rpc_port = 0;
    unsigned int _keyid = 0;
    int _connect_type = 0;
    int _data_type = 2;
    eAVClient* _remote_client = nullptr;
    eAVClient* _cmd_client = nullptr;
    //HANDLE _hExitEvent = NULL;
    std::map<unsigned int, RecvCALLBACK> _recvCBMap;
    std::map<unsigned int, FailCALLBACK> _failCBMap;
    RemoteStatus _status = RemoteStatus::DISCONNECT;
    std::string _token, _guid, _lang;
    short _conn_type = 0;

    DATA_PACKAGE _package;
    std::vector<DATA_PACKAGE> _recv_packages;
    std::vector<unsigned char> _head_size; //防止头部数据不全

    int _remain = 0;
    int _len = 0;
    long long _alive_ts_sec = 0;
    int64_t _winid = 0;
    int _gl_w_win = 0;
    int _gl_h_win = 0;
};

#endif // ECONTROLTRANS_H
