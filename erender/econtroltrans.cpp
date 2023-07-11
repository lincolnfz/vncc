#include "econtroltrans.h"
#include "base/misctool.h"
#include <boost/functional/hash.hpp>
#include <iostream>
#include "common/eInstructions.h"
#include "base/algorithm/CMD5Ex.h"
#include "ePrivateProto.h"
#include <unistd.h>
#include <qlogging.h>
#include "eAVRender.h"
#include <base/algorithm/Base64.h>

bool gDone = false;
bool g_play_logo_animation = true;
bool g_multi_win_platform = true;
std::string g_lbh_token;
std::string g_acess_token;
int g_modelID = 0;

static bool cmp(const std::string& a, const std::string& b) {
    return a.compare(b) < 0;
}

std::string getSign(const nlohmann::json& root)
{
    nlohmann::json data = root["data"];
    std::stringstream ss;
    ss << root["time"];
    std::string strTime = ss.str();

    std::vector<std::string> keys;
    for (auto item : data.items()) {
        keys.push_back(item.key());
    }
    std::string values;
    values.append(strTime.substr(0, 4));

    std::sort(keys.begin(), keys.end(), cmp);
    for (auto it = keys.begin(); it != keys.end(); ++it) {
        if (!(data[it->c_str()].is_object())) {
            std::stringstream ss;
            ss << data[it->c_str()];
            values.append(ss.str());
        }
    }
    values.append(strTime.substr(strTime.size() - 4));
    std::string strMd5;
    CMD5Ex md5;
    md5.GenerateCMD5Ex((unsigned char*)values.c_str(), values.size());
    strMd5 = md5.ToString();
    return strMd5;
}

eControlTrans* gTrans = nullptr;
void eControlTrans::Init() {
    gTrans = this;
    _remote_client = new eAVClient;
    _cmd_client = new eAVClient;
    _remote_client->Init();
    _cmd_client->Init();
    _remote_client->SetConnectCB(eControlTrans::rt_RemoteConnectCB, nullptr);
    _remote_client->SetErrorCB(eControlTrans::rt_ErrCB, nullptr);
    _cmd_client->SetConnectCB(eControlTrans::RemoteConnectCB, nullptr);
    _cmd_client->SetRecvCB(eControlTrans::recvRemoteDataCB, nullptr);
    _cmd_client->SetErrorCB(eControlTrans::ErrCB, nullptr);
    _render = std::make_shared<eAVRender>(ThreadId::kThreadDecode, "decodethd", "15486");
    nbase::ThreadManager::RegisterThread((int)ThreadId::kThreadDecode, _render);

    _glrenderThd = std::make_shared<RenderHelpThrad>();
    nbase::ThreadManager::RegisterThread((int)ThreadId::kThreadGL, _glrenderThd);
    _glrenderThd->Start();

    //_render.reset(new eAVRender(ThreadId::kThreadDecode, "render", _orderid.c_str()));
    //_render->set_thread_priority(nbase::ThreadPriority::kThreadPriorityHigh);
    _render->setParm(_av_url.c_str(), _ws_url.c_str(), _stun_url.c_str(), _connect_type, _data_type);
    if (true/*IsWindow(_glwnd)*/) {
        _render->SetExtInfo(_token.c_str(), _guid.c_str(), (void*)_winid, _gl_w_win, _gl_h_win);
    }
    if (true) {
        //非多端直接连视频
        nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl,
            nbase::Bind(&eControlTrans::StartRender, gTrans));
    }
    int ntimes = 0;
    while (ntimes < 50) {
        if (eAVClient::s_base) {
            nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl,
                nbase::Bind(&eControlTrans::HelpRemoteRun, this));
            qDebug("eAVClient::s_base\n");
            break;
        }
        else {
            //WaitForSingleObject(eAVClient::s_hReadyEvent, 20);
            usleep(20);
            ++ntimes;
        }
    }
}

void eControlTrans::Cleanup() {
    //SetThreadWasQuitProperly(false);
    //nbase::ThreadManager::UnregisterThread();
}

void eControlTrans::DoOp(std::string json) {

}

void eControlTrans::close_self() {
    gDone = true;
    qDebug("close_self\n");
    //gTrans->_render->Stop();
    if (gTrans->_cmd_client->Disconnect()) {
        //WaitForSingleObject(gTrans->_hExitEvent, 6000);
    }
    //SetEvent(gEvent);
}

void eControlTrans::TransMouse(std::string strjson) {
    long ltime = getNowTickMs();
    nlohmann::json jRoot, jData;
    jData = nlohmann::json::parse(strjson, nullptr, true);
    jRoot["data"] = jData;
    jRoot["from"] = 3;
    jRoot["type"] = 1;
    jRoot["time"] = ltime;
    jRoot["sign"] = "";// getSign(jRoot);

    std::string msg = sProto.GenNormal(CMD_ACTION_TOUCH, strjson, "");
    nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl,
        nbase::Bind(&eControlTrans::SafeSendRemoteMsg, gTrans, msg) );
}

void eControlTrans::SendEventMsg(std::string strjson, int type) {
    long ltime = getNowTickMs();

    std::string msg = sProto.GenNormal(type, strjson, g_lbh_token.c_str()); //jRoot.dump();
    nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl,
       nbase::Bind(&eControlTrans::SafeSendCmdMsg, gTrans, msg));
}

void eControlTrans::Roate(int w, int h, float angle) {
    nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl,
        nbase::Bind(&eControlTrans::SaveRoate, gTrans, w, h, angle));
}

int g_Bitrate = 2000;
bool g_bAutorate = false;
extern void setbitrate(bool bauto, int bitrate);
extern void setfps(int fps);

void eControlTrans::Bitrate(int bitrate, int bauto) {
    g_Bitrate = bitrate;
    g_bAutorate = !!bauto;
    //bupate_quality = true;
    setbitrate(g_bAutorate, g_Bitrate);
}

void eControlTrans::SetFps(int fps) {
    setfps(fps);
}

void eControlTrans::Preview(bool) {
    nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl,
        nbase::Bind(&eControlTrans::HelpPreview, gTrans));
}

void eControlTrans::ScreenCap(long user_order_id,  std::string out_filepath) {
    nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl,
        nbase::Bind(&eControlTrans::help_ScreenCap, gTrans, user_order_id, out_filepath));
}

void eControlTrans::DoAlive() {

}

void eControlTrans::DoRoate(int w, int h, float angle) {
    nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl,
        nbase::Bind(&eControlTrans::SaveRoate, this, w, h, angle));
}

void eControlTrans::SaveRoate(int w, int h, float angle) {
    nbase::StdClosure fn = ToWeakCallback(nbase::Bind([this](int w, int h, float ang)->void {
            if (_render && _render->_gl_render) {
                _render->_gl_render->rotate(w, h, ang);
            }
            }, w, h, angle));
        nbase::ThreadManager::PostTask((int)ThreadId::kThreadGL, fn);
}

void eControlTrans::SafeSendRemoteMsg(std::string msg) {
    if (_status == RemoteStatus::CONNECTED) {
        _remote_client->SendData((unsigned char*)(msg.c_str()), msg.size());
    }
    else {
        qDebug("remote not connect \n");
    }
}

void eControlTrans::SafeSendCmdMsg(std::string msg) {
    if (_status == RemoteStatus::CONNECTED) {
        /// <summary>
        /// 因为服务器有bug,所以发送数据时都sleep(1ms).输入法发现在问题
        /// </summary>
        //usleep(1);
        _cmd_client->SendData((unsigned char*)(msg.c_str()), msg.size());
    }
    else {
        qDebug("remote not connect \n");
    }
}

void eControlTrans::RemoteControlRun() {
     size_t pos = _remote_url.find(":");
     std::string strurl = _remote_url.substr(0, pos);
     std::string strport = _remote_url.substr(pos + 1);
     int port = atoi(strport.c_str());
     _remote_client->Connect(strurl.c_str(), port);

     pos = _cmd_url.find(":");
     strurl = _cmd_url.substr(0, pos);
     strport = _cmd_url.substr(pos + 1);
     port = atoi(strport.c_str());
     _cmd_client->Connect(strurl.c_str(), port);
     /*if (!gDone) {
         gTrans->_status = RemoteStatus::DISCONNECT;
         srand(time(0));
         nbase::ThreadManager::PostDelayedTask((int)ThreadId::kThreadRemoteControl,
             nbase::Bind(&eControlTrans::HelpRemoteRun, this),
             nbase::TimeDelta::FromSeconds(rand()%6 + 1));
     }
     else {
         SetEvent(_hExitEvent);
     }*/
}

void eControlTrans::HelpRemoteRun() {
    //std::thread thd(&eControlTrans::RemoteControlRun, this);
    //thd.detach();
    RemoteControlRun();
}

void eControlTrans::HelpErrCB() {
    if (gDone) {
        //SetEvent(_hExitEvent);
    }
}

void eControlTrans::RecvData(std::vector<unsigned char>& data) {

    constexpr int HEAD_SIZE = 4;
    while (data.size()) {
        if (_len == 0) {
            //新的数据块
            if (_head_size.size() > 0) {
                int r = HEAD_SIZE - _head_size.size();
                _head_size.insert(_head_size.end(), data.begin(), data.begin() + r);
                unsigned char* head = &(_head_size[0]);
                memcpy(&_len, head, HEAD_SIZE);
                _len = ntohl(_len);
                data.erase(data.begin(), data.begin() + r);
                _head_size.clear();
            }
            else {
                if (data.size() < 4) {
                    _head_size = data;
                    //头部数据不全
                    break;
                }
                else {
                    //获取头部数据
                    unsigned char* head = &(data[0]);
                    memcpy(&_len, head, HEAD_SIZE);
                    _len = ntohl(_len);
                    data.erase(data.begin(), data.begin() + HEAD_SIZE);
                }
            }


            _remain = _len;
            _package.clear();
            if (data.size() >= _remain) {
                //收到完整个的包
                _package.insert(_package.end(), data.begin(), data.begin() + _remain);
                data.erase(data.begin(), data.begin() + _remain);
                _remain = 0;
                _len = 0;
                if (_remain == 0) {
                    _package.push_back('\0');
                    RouteCmd((char*)(&_package[0]));
                    _package.clear();
                }
            }
            else {
                //不够收完全的包
                _package.insert(_package.end(), data.begin(), data.end());
                _remain -= data.size();
                data.erase(data.begin(), data.end());
            }
        }
        else {
            if (data.size() >= _remain) {
                //收到完整个的包
                _package.insert(_package.end(), data.begin(), data.begin() + _remain);
                data.erase(data.begin(), data.begin() + _remain);
                _remain = 0;
                _len = 0;
                if (_remain == 0) {
                    _package.push_back('\0');
                    RouteCmd((char*)(&_package[0]));
                    _package.clear();
                }
            }
            else {
                //不够收完全的包
                _package.insert(_package.end(), data.begin(), data.end());
                _remain -= data.size();
                data.erase(data.begin(), data.end());
            }
        }
    }
}

void eControlTrans::SendHeart() {
    /*nlohmann::json jRoot, jData;
    static bool bIsInit = true;
    jRoot["type"] = 6;
    jRoot["from"] = 3;

    jData["command"] = "keepalive";
    jData["init"] = bIsInit ? 1 : 0;
    jRoot["data"] = jData;

    jRoot["time"] = getSysTime();
    bIsInit = false;*/
    std::string msg = sProto.Heartbeat(g_lbh_token.c_str()); //jRoot.dump();
    _cmd_client->SendData((unsigned char*)(msg.c_str()), msg.size());
    _remote_client->SendData((unsigned char*)(msg.c_str()), msg.size());
    nbase::ThreadManager::PostDelayedTask((int)ThreadId::kThreadRemoteControl,
        nbase::Bind(&eControlTrans::SendHeart, this),
        nbase::TimeDelta::FromSeconds(30) );
}

void eControlTrans::StartRender() {
    _render->Start();
}

void eControlTrans::SendClearApps(){
    long ltime = getNowTickMs();
    gTrans->_status = RemoteStatus::CONNECTED;
    nlohmann::json jRoot, jData, jCmdData;
    //com.dkp.windowslauncher com.cloudecalc.launcher3
    /*jData["command"] = "launcher";
    jData["data"] = "{\"packageName\":\"com.dkp.windowslauncher\",\"type\":\"windows\"}";
    jData["time"] = ltime;

    jRoot["type"] = 2;
    jRoot["from"] = 1;
    jRoot["data"] = jData;
    jRoot["time"] = ltime;
    jRoot["token"] = "";
    jRoot["sign"] = getSign(jRoot);
    std::string msg = jRoot.dump();*/

    jData["packageName"] = "com.dkp.windowslauncher";
    jData["type"] = "windows";
    jData["width"] = 1920;
    jData["height"] = 1080;
    //jCmdData["data"] = jData;
    std::string msg = jData.dump();
    std::string out = sProto.GenNormal(130001, msg, g_lbh_token.c_str()); //CMD_ACTION_MULTI_PREPARE = 130001

    //_remote_client.SendData((unsigned char*)(msg.c_str()), msg.size());
    SendTcpData("launcher", (unsigned char*)(out.c_str()), out.size(), [=](const char* msg)->void {
        nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl,
            nbase::Bind(&eControlTrans::SendSetProp, gTrans, "0"));
        nbase::ThreadManager::PostDelayedTask((int)ThreadId::kThreadRemoteControl,
            nbase::Bind(&eControlTrans::SendAmBroadcast, gTrans),
            nbase::TimeDelta::FromSeconds(10));
        //_render->Start();
        //gTrans->_status = RemoteStatus::CONNECTED;
        },
        [=]()->void{
            int i = 0;
        }
        );

    //nbase::ThreadManager::PostDelayedTask((int)ThreadId::kThreadRemoteControl,
    //    nbase::Bind(&eControlTrans::StartRender, gTrans),
    //    nbase::TimeDelta::FromMilliseconds(4000));
    nbase::StdClosure fn = ToWeakCallback(nbase::Bind([=]()->void{
        _render->triggerStraamThread();
    }));
    nbase::ThreadManager::PostDelayedTask((int)ThreadId::kThreadRemoteControl,
        fn,
        nbase::TimeDelta::FromMilliseconds(4000));
}

void eControlTrans::SendSetProp(std::string szMode)
{
    long ltime = getNowTickMs();
    nlohmann::json jRoot, jData, jCmdData;

    {
        nlohmann::json array, data;
        data["key"] = "android.screen.mode";
        data["value"] = szMode;
        array.push_back(data);
        jData["data"] = array.dump();
    }

    jData["command"] = "setprop";
    jData["time"] = ltime;

    jRoot["type"] = 2;
    jRoot["from"] = 1;
    jRoot["data"] = jData;
    jRoot["time"] = ltime;
    jRoot["token"] = "";
    jRoot["sign"] = getSign(jRoot);

    std::string msg = jRoot.dump();
    //_cmd_client->SendData((unsigned char*)(msg.c_str()), msg.size());
}

void eControlTrans::SendAmBroadcast()
{
    long ltime = getNowTickMs();
    nlohmann::json jRoot, jData;

    {
        nlohmann::json data;
        data["action"] = "android.app.action.orientation.change";
        data["extrakey"] = "test";
        data["extravalue"] = "test";
        jData["data"] = data.dump();
    }

    jData["command"] = "amBroadcast";
    jData["time"] = ltime;

    jRoot["type"] = 2;
    jRoot["from"] = 1;
    jRoot["data"] = jData;
    jRoot["time"] = ltime;
    jRoot["token"] = "";
    jRoot["sign"] = getSign(jRoot);

    std::string msg = jRoot.dump();
    //_cmd_client->SendData((unsigned char*)(msg.c_str()), msg.size());
}

void eControlTrans::SendSwitchLang() {
    /*LONGLONG ltime = getSysTime();
    nlohmann::json jRoot, jData, data;

    data["language"] = _lang;
    data["cusType"] = 4;
    jData["command"] = "repareConnectYun";
    jData["data"] = data.dump();

    jRoot["type"] = 2;
    jRoot["from"] = 1;
    jRoot["data"] = jData;
    jRoot["time"] = ltime;
    jRoot["token"] = "";
    jRoot["sign"] = getSign(jRoot);*/
    //std::string msg = sProto.SwitchLang(_lang.c_str()); //jRoot.dump();

    std::string msg = sProto.Connect(_lang.c_str(), g_acess_token.c_str(), g_modelID, g_lbh_token.c_str());
    //_cmd_client->SendData((unsigned char*)(msg.c_str()), msg.size());
}

void eControlTrans::QueryDeviceInfo() {
    long ltime = getNowTickMs();
    nlohmann::json jRoot, jData, data;

    jData["command"] = "getDeviceinfo";
    jData["data"] = ltime;

    jRoot["type"] = 2;
    jRoot["from"] = 1;
    jRoot["data"] = jData;
    jRoot["time"] = ltime;
    jRoot["token"] = "";
    jRoot["sign"] = getSign(jRoot);
    std::string msg = jRoot.dump();
    _cmd_client->SendData((unsigned char*)(msg.c_str()), msg.size());
}

void eControlTrans::RemoteConnectCB(void* user_arg) {
    gTrans->SendHeart();
    if (g_multi_win_platform) {
        nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl,
            nbase::Bind(&eControlTrans::SendClearApps, gTrans));
    }
    /*nbase::ThreadManager::PostDelayedTask((int)ThreadId::kThreadRemoteControl,
        nbase::Bind(&eControlTrans::SendSetProp, gTrans, "0"),
        nbase::TimeDelta::FromSeconds(6));
    nbase::ThreadManager::PostDelayedTask((int)ThreadId::kThreadRemoteControl,
        nbase::Bind(&eControlTrans::SendAmBroadcast, gTrans),
        nbase::TimeDelta::FromSeconds(9));*/
    //gTrans->SendClearApps();
    //gTrans->SendSetProp("0");
    //gTrans->SendAmBroadcast();
    /*static bool bfirst = true;
    if (bfirst) {
        const char* first = "{\"data\":{\"command\":\"clearApps\", \"data\" : \"[\"com.cloudecalc.launcher3\"]\", \"time\" : 1654849017786}, \"from\" : 1, \"sign\" : \"037ADE06B24688128F68AAB993F97F3B\", \"time\" : 1654849017786, \"token\" : \"\", \"type\" : 2})";
        gTrans->_remote_client.SendData((unsigned char*)first, strlen(first));
        bfirst = false;
    }*/
    gTrans->SendSwitchLang();
    //连接视频流的工作与连反控分开执行
    //nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl,
    //    nbase::Bind(&eControlTrans::StartRender, gTrans));
    //gTrans->SendCollectData(std::string("connect_msg_success"), std::string("{}"), false);
    gTrans->SetConnState(CMD_CONN);
    gTrans->_status = RemoteStatus::CONNECTED;
}

void eControlTrans::recvRemoteDataCB(const unsigned char* data, const int len, void* user_arg) {
    std::vector<unsigned char> vecdata(data, data+len);
    nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl, nbase::Bind(&eControlTrans::RecvData, gTrans, vecdata));
}

void eControlTrans::ErrCB(eAVClient::ERR_CODE, void* user_arg) {
    gTrans->SendCollectData(std::string("connect_msg_error"), std::string("{}"), false);
    gTrans->RemoveConnState(CMD_CONN);
    nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl, nbase::Bind(&eControlTrans::HelpErrCB, gTrans));
}

void eControlTrans::rt_RemoteConnectCB(void* user_arg) {
    //gTrans->SendCollectData(std::string("connect_touch_success"), std::string("{}"), false);
    gTrans->SetConnState(RT_CONN);
}

void eControlTrans::rt_ErrCB(eAVClient::ERR_CODE, void* user_arg) {
    //gTrans->SendCollectData(std::string("connect_touch_error"), std::string("{}"), false);
    gTrans->RemoveConnState(RT_CONN);
}

void eControlTrans::SendAVData(const char* fn_name, std::vector<unsigned char>& data, int size, int width, int height, int pixfmt) {
    if (!gDone) {
        try {
            //_client->async_call(fn_name, _keyid, data, size, width, height, pixfmt);
        }
        catch (...) {
            int i = 0;
        }
    }
    else {
        qDebug("Do not SendAVData\n");
    }
}

void eControlTrans::goDisconnect(const char* fn_name, const int disconnect_type) {
    if (!gDone) {
        try {
            //_client->async_call(fn_name, _keyid, disconnect_type);
        }
        catch (...) {
            int i = 0;
        }
    }
    else {
        //printf("Do not SendAVData\n");
    }
}

void eControlTrans::SendCollectData(const std::string datatype, const std::string data, const bool bhighfeq) {

}

bool eControlTrans::SendTcpData(const char* name, const unsigned char* data, const int len, RecvCALLBACK succ, FailCALLBACK fail) {
    //以前请求还存在,就不再发送;
    bool ret = false;
    boost::hash<std::string> string_hash;
    unsigned int hash_val = string_hash(name);
    short flag = 0;
    auto got = _recvCBMap.find(hash_val);
    if (got == _recvCBMap.end()) {
        flag |= 0x1;
    }
    auto got1 = _failCBMap.find(hash_val);
    if (got1 == _failCBMap.end()) {
        flag |= 0x2;
    }
    if (flag == 0x3) {
        _recvCBMap.emplace(hash_val, succ);
        _failCBMap.emplace(hash_val, fail);
        ret = _cmd_client->SendData(data, len);
    }
    if (!ret) {
        _recvCBMap.erase(hash_val);
        _failCBMap.erase(hash_val);
    }
    else {

        nbase::ThreadManager::PostDelayedTask((int)ThreadId::kThreadRemoteControl,
            nbase::Bind(&eControlTrans::RemoveCB, this, hash_val) ,
            nbase::TimeDelta::FromSeconds(30)
            );
    }

    return ret;
}

void eControlTrans::RouteCmd(const char* szjson) {
    BASE_PROTO proto;
    QuickGetJsonData(szjson, proto);
    int cmdType = proto.cmdType;
    if (cmdType == CMD_ACTION_CLOSE_CLIENT) {
        gTrans->RemoveConnState(AV_CONN);
        if (g_multi_win_platform) {
            //bStopRender = true;
            std::string& data = proto.data;
            int do_type = atoi(data.c_str());
            gTrans->goDisconnect("Eum_Contros", do_type);
        }
        else {
            std::string& data = proto.data;
            int do_type = atoi(data.c_str());
            if (do_type == 2 || do_type == 3)
            {
                //gTrans->goDisconnect("jm_disconnect", EXIT_BACKUP_RESTORE);
            }
            else
            {
                //gTrans->goDisconnect("jm_disconnect", EXIT_OCCUPIED);
            }

        }
    }
    else if (cmdType == CMD_ACTION_ROATE_CLIENT) {
        return;
    }
    else if (cmdType == CMD_ACTION_COPY_CUT_RESULT) {
        std::wstring clip_data = nbase::UTF8ToUTF16(proto.data);
        //SetClipboard(clip_data.c_str());
    }
    else if (cmdType == CMD_ACTION_PREPARE_CONNECT) {
        //if (g_multi_win_platform && g_play_logo_animation == false) {
            //多端切换桌面根据此条信息
            //nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl,
            //    nbase::Bind(&eControlTrans::StartRender, gTrans));
        //}
        //_render->triggerStraamThread();
        int i = 0;
    }
}

void eControlTrans::DecodeBase64_2_File(const char* pbase64) {

}

void eControlTrans::RemoveCB(unsigned int val) {
    _recvCBMap.erase(val);
    auto got = _failCBMap.find(val);
    if (got != _failCBMap.end()) {
        got->second();
        _failCBMap.erase(got);
    }
}

void eControlTrans::SetConnState(short type) {
    nbase::StdClosure fn = ToWeakCallback(nbase::Bind([=](short type)->void {
        if (!gDone) {
            _conn_type |= type;
            qDebug("conn state %d, %d\n", _conn_type, type);
            if (_conn_type == FULL_CONN) {
                SendCollectData("connect_comp", "{}", false);
            }
        }
        else {
            qDebug("Do not SetConnState\n");
        }
        }, type));
    nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl, fn);
}

void eControlTrans::RemoveConnState(short type) {
    nbase::StdClosure fn = ToWeakCallback(nbase::Bind([=](short type)->void {
        if (!gDone) {
            _conn_type &= ~type;
        }
        else {
            qDebug("Do not RemoveConnState\n");
        }
        }, type));
    nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl, fn);
}

void eControlTrans::SendFrameActivate() {

}

void eControlTrans::SetWindow(uint64_t winid, int w, int h){
    //_render = std::make_shared<eAVRender>((int)ThreadId::kThreadDecode, "decodethd", "15486");
    //_render->SetExtInfo("", "", (void*)winid, w, h);
    _winid = winid;
    _gl_w_win = w;
    _gl_h_win = h;
}

void eControlTrans::HelpPreview() {
   //_render->StopVideo();
}

void eControlTrans::help_ScreenCap(long user_order_id, std::string out_filepath) {
    //user_o_id = user_order_id;
    //pic_dir = out_filepath;
    //std::string strparm = sProto.ScreenCaptrue(0, 0, true, 1.0, 1.0, true, g_lbh_token.c_str());
    //_cmd_client->SendData((unsigned char*)(strparm.c_str()), strparm.size());
}

void eControlTrans::SendResolution(int width, int height) {

}

void eControlTrans::UpdateRatio(int l, int s) {

}

void eControlTrans::SendRotate(int rotate, int width, int height) {

}

void eControlTrans::CheckAlive() {
    nbase::StdClosure fn = ToWeakCallback(nbase::Bind([=]()->void {
        long long esp = getNowTickMs() / 1000 - _alive_ts_sec;
        if (esp < 60) {
            CheckAlive();
        }
        else {
            close_self();
        }
        }));
    nbase::ThreadManager::PostDelayedTask((int)ThreadId::kThreadRemoteControl, fn, nbase::TimeDelta::FromSeconds(30));
}

void eControlTrans::GoDebug() {
    freopen("CONOUT$", "w", stdout);
}

void eControlTrans::SendMouseMsg(float x, float y, int mtype){
    nbase::StdClosure fn = ToWeakCallback(nbase::Bind([=](float x, float y ,  int mtype)->void{
        std::string strjson = sProto.MouseAction(x, y, mtype);
        std::string msg = sProto.GenNormal(CMD_ACTION_TOUCH, strjson, "");
        SafeSendRemoteMsg(msg);
    }, x, y, mtype));
    nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl, fn);
}

void eControlTrans::SendTextMsg(std::string& txt){
    nbase::StdClosure fn = ToWeakCallback(nbase::Bind([=](std::string& txt)->void{
        std::string msg = sProto.GenNormal(CMD_ACTION_IME_INPUT_TEXT, txt, g_lbh_token.c_str());
        SafeSendCmdMsg(msg);
    }, txt));
    nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl, fn);
}

void eControlTrans::SendLogEvent(std::string& txt){
    nbase::StdClosure fn = ToWeakCallback(nbase::Bind([=](std::string& txt)->void{
        std::string msg = sProto.GenNormal(CMD_ACTION_SHELL, txt, g_lbh_token.c_str());
        SafeSendCmdMsg(msg);
    }, txt));
    nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl, fn);
}

void eControlTrans::RButtonEvent(float x, float y, int mousetype) {
    nlohmann::json jData;
    nlohmann::json jBoast, item, event_data;
    item["x"] = x;
    item["y"] = y;
    item["mousetype"] = mousetype;

    std::string mouse_data = item.dump().c_str();
    int nBase64Len = CBase64::Base64Encode(mouse_data.c_str(), mouse_data.size(), nullptr, 0);
    char* pcsBase64 = (char*)calloc(nBase64Len + 1, sizeof(char));
    CBase64::Base64Encode(mouse_data.c_str(), mouse_data.size(), (BYTE*)pcsBase64, nBase64Len);
    mouse_data = pcsBase64;
    free(pcsBase64);

    event_data["key"] = "event_data";
    event_data["value"] = mouse_data;

    jBoast["action"] = "android.event.action.MOUSE_EVENT";
    jBoast["extraInfos"].push_back(event_data);
    std::string mousemsg = jBoast.dump();
    nbase::StdClosure fn = ToWeakCallback(nbase::Bind([=](std::string mousemsg)->void{
        std::string msg = sProto.GenNormal(CMD_ACTION_SEND_BROADCASTRECEIVER, mousemsg, "");
        SafeSendCmdMsg(msg);
    }, mousemsg));
    nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl, fn);
}
