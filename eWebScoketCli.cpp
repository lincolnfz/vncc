#include "eWebScoketCli.h"
#include "json.hpp"
#include "common/defThread.h"
//#include "../common/eEumExitCode.h"
#include <QDebug>
#include "erpcmanager.h"

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

void eWebScoketCli::on_open(websocketpp::connection_hdl hdl) {
    //std::string str_login = R"({"ope":"login","Device":"window"})";
    //_cli.send(hdl, str_login, websocketpp::frame::opcode::text);
    LoginWindow();
    //nbase::ThreadManager::PostDelayedTask((int)ThreadId::kThreadGlobalMisc, nbase::Bind(&eWebScoketCli::on_heart, this),
    //    nbase::TimeDelta::FromSeconds(60));
    _cli->set_timer(10000, [=](websocketpp::lib::error_code const &)->void{
        eWebScoketCli::on_heart();
    });
}

void eWebScoketCli::on_close(websocketpp::connection_hdl hdl) {
    if(!_bDone){
        ReConnect();
    }else{
        qDebug() << "on_close";
    }
}

void eWebScoketCli::on_fail(websocketpp::connection_hdl hdl) {
    ReConnect();
}

bool eWebScoketCli::on_validate(websocketpp::connection_hdl hdl) {
    int i = 0;
    return true;
}

void eWebScoketCli::Show_btns_slot(QString clients){
    std::string strclients = clients.toStdString();
    qDebug() << "clients:" << QString(strclients.c_str());
    sRpcManager.ShowBtns(strclients);
}

// This message handler will be invoked once for each incoming message. It
// prints the message and then sends a copy of the message back to the server.
void eWebScoketCli::on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg) {
    std::cout << "on_message called with hdl: " << hdl.lock().get()
        << " and message: " << msg->get_payload()
        << std::endl;
    std::string strtmp = nbase::StringPrintf("recv: %s\n", msg->get_payload().c_str());
    if (strcmp(msg->get_payload().c_str(), "show") == 0) {
        nbase::StdClosure fn = ToWeakCallback(nbase::Bind([this]()->void {
            //call render
            emit sWebsocket.Launch_signal();
            }));
        //nbase::ThreadManager::PostTask((int)ThreadId::kThreadUI, fn);
    }
    else if (strstr(msg->get_payload().c_str(), "close")) {
        nbase::StdClosure fn = ToWeakCallback(nbase::Bind([this]()->void {
            //close render

            //close render
            Stop();
            }));
        //nbase::ThreadManager::PostTask((int)ThreadId::kThreadUI, fn);
    }
    else if (strstr(msg->get_payload().c_str(), "isexist")) {
        nlohmann::json root;
        try {
            root = nlohmann::json::parse(msg->get_payload().c_str(), nullptr, true);
            if (root.find("isexist") != root.end() && !(root.at("isexist").is_null())) {
                bool exist = false;
                root.at("isexist").get_to(exist);
                nbase::StdClosure fn = ToWeakCallback(nbase::Bind([this](bool exist)->void {
                    //dummy
                    }, exist));
                //nbase::ThreadManager::PostTask((int)ThreadId::kThreadUI, fn);
            }
        }
        catch (nlohmann::json::parse_error& e) {
        }
        catch (...) {

        }
    }
    else if (strstr(msg->get_payload().c_str(), "clients")) {
        nlohmann::json root;
        try {
            root = nlohmann::json::parse(msg->get_payload().c_str(), nullptr, true);
            if (root.find("clients") != root.end() && !root.at("clients").is_null()) {
                std::string strclients = root["clients"];
                emit this->ShowBtns_signal(QString(strclients.c_str()));

            }
        }
        catch (nlohmann::json::parse_error& e) {
        }
        catch (...) {

        }
    }
    /*else {
        nlohmann::json jRoot;
        try {
            std::string ope;
            bool ret = false;
            jRoot = nlohmann::json::parse(msg->get_payload().c_str(), nullptr, true);
            if (jRoot.find("ope") != jRoot.end() && !jRoot.at("ope").is_null()) {
                ope = jRoot["ope"];
            }
            if (jRoot.find("Result") != jRoot.end() && !jRoot.at("Result").is_null()) {
                ret = jRoot["Result"];
            }
            //if(ret && )
        }
        catch (nlohmann::json::parse_error& e) {

        }
        catch (...) {

        }
    }*/
    /*else if (strcmp(msg->get_payload().c_str(), "quit") == 0) {
        StdClosure fn = nbase::Bind([this]()->void {
            if (g_demo_emu_win && IsWindow(g_demo_emu_win->GetHWND())) {
                g_demo_emu_win->DoDemoExit();
            }
            });
        nbase::ThreadManager::PostTask((int)ThreadId::kThreadUI, this->ToWeakCallback(fn));
    }*/

    /*websocketpp::lib::error_code ec;

    c->send(hdl, msg->get_payload(), msg->get_opcode(), ec);
    if (ec) {
        std::cout << "Echo failed because: " << ec.message() << std::endl;
    }*/
}

void eWebScoketCli::on_heart() {
    if(_bDone){
        return;
    }
    nlohmann::json root;
    root["ope"] = "heart";
    root["userid"] = _strUser;
    std::string str_heart = root.dump();  // R"({"ope":"heart" })";
    try {
        _cli->send(m_hdl, str_heart, websocketpp::frame::opcode::text);
    }
    catch (websocketpp::exception const& e) {
        std::cout << e.what() << std::endl;
    }

    //void(lib::error_code const &)
    _cli->set_timer(10000, [=](websocketpp::lib::error_code const &)->void{
        eWebScoketCli::on_heart();
    });
    //nbase::ThreadManager::PostDelayedTask((int)ThreadId::kThreadGlobalMisc, nbase::Bind(&eWebScoketCli::on_heart, this),
    //    nbase::TimeDelta::FromSeconds(10));
}

void eWebScoketCli::ReConnect() {
    if (!_bDone) {
        srand(time(0));
        nbase::StdClosure fn = ToWeakCallback(nbase::Bind([this]()->void {
            _cli->stop();
            std::thread thd(&eWebScoketCli::Run, this, _strWS_uri.c_str(), _strUser.c_str());
            thd.detach();
            }));
        //nbase::ThreadManager::PostDelayedTask((int)ThreadId::kThreadGlobalMisc, fn,
         //   nbase::TimeDelta::FromSeconds(rand()%6+1));
        _cli->set_timer(7000, [=](websocketpp::lib::error_code const &)->void{
            //eWebScoketCli::on_heart();
            _cli->stop();
            std::string code;
            _cli->close(m_hdl, websocketpp::close::status::normal, code);
            std::thread thd(&eWebScoketCli::Run, this, _strWS_uri.c_str(), _strUser.c_str());
            thd.detach();
        });
    }
}

eWebScoketCli::eWebScoketCli() {
    //eWebScoketCli::g_inst = this;
    connect(this, &eWebScoketCli::ShowBtns_signal, this, &eWebScoketCli::Show_btns_slot);
}


eWebScoketCli::~eWebScoketCli() {

}

bool eWebScoketCli::Run(const char* ws, const char* user) {
    bool ret = false;
    _bRun = true;
    _bDone = false;
    _bStopDone = false;
    _strUser = user;
    _strWS_uri = ws;
    _cli = std::make_unique<client>();
    try {
        // Set logging to be pretty verbose (everything except message payloads)
        _cli->set_access_channels(websocketpp::log::alevel::all);
        _cli->clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize ASIO
        _cli->init_asio();

        // Register our message handler
        _cli->set_message_handler(bind(&eWebScoketCli::on_message, this, _cli.get(), ::_1, ::_2));
        _cli->set_open_handler(bind(&eWebScoketCli::on_open, this, ::_1));
        _cli->set_close_handler(bind(&eWebScoketCli::on_close, this, ::_1));
        _cli->set_fail_handler(bind(&eWebScoketCli::on_fail, this, ::_1));
        _cli->set_validate_handler(bind(&eWebScoketCli::on_validate, this, ::_1));

        websocketpp::lib::error_code ec;
        client::connection_ptr con = _cli->get_connection(_strWS_uri, ec);
        if (ec) {
            //std::cout << "could not create connection because: " << ec.message() << std::endl;
            //return 0;
            ret = false;
        }
        else {
            ret = true;
            std::thread thd(&eWebScoketCli::loop, this);
            thd.detach();
        }
    }
    catch (websocketpp::exception const& e) {
        std::cout << e.what() << std::endl;
    }catch(...){
        std::cout << "unknow" << std::endl;
    }

    return ret;
}

void eWebScoketCli::Stop() {
    if (!_bRun)
        return;
    if (_cli.get() == nullptr)
        return;

    std::unique_lock<std::mutex> lock (_clock_lock);
    if(_bStopDone == true){
        return;
    }
    _bDone = true;
    _bStopDone = true;
    try {
        //_cli->stop();
        std::string bye = "bye";
        std::string code;
        websocketpp::lib::error_code ec;
        //client::connection_ptr con = _cli->get_connection(_strWS_uri, ec);
        std::string reason;
        _cli->stop();
        _cli->close(m_hdl, websocketpp::close::status::normal, code);
        int i = 0;
    }
    catch (websocketpp::exception const& e) {
        std::cout << e.what() << std::endl;
    }catch(...){
        std::cout << "unknow" << std::endl;
    }
    
    //_cli.close(m_hdl, )
    _thread_con.wait_for(lock, std::chrono::milliseconds(2000));
    qDebug() << "stop done";
}

void eWebScoketCli::LoginWindow() {
    nlohmann::json jRoot;
    jRoot["ope"] = "login";
    jRoot["Device"] = "qilin";
    jRoot["userid"] = _strUser;
    std::string str_login = jRoot.dump();
    try {
        _cli->send(m_hdl, str_login, websocketpp::frame::opcode::text);
    }
    catch (websocketpp::exception const& e) {
        std::cout << e.what() << std::endl;
    }
}

void eWebScoketCli::SwitchDevice(DEVICE device) {
    static std::string devices[] = {"phone","pad","tv","box","ios"};
    SwitchDevice(devices[(int)device]);
}

void eWebScoketCli::SwitchDevice(std::string device){
    static std::string devices[] = {"phone","pad","tv","box","ios"};
    char szmsg[96] = {0};
    sprintf(szmsg, R"({ "ope":"goto", "DeviceType":"%s"})", device.c_str());
    nlohmann::json jRoot;
    jRoot["ope"] = "goto";
    jRoot["DeviceType"] = device;
    jRoot["userid"] = _strUser;
    std::string msg;
    msg = jRoot.dump();
    SafeSend(msg);
}

void eWebScoketCli::SafeSend(std::string msg){
    if(!_bDone){
        _cli->set_timer(0, [=](websocketpp::lib::error_code const &)->void{
            try {
                _cli->send(m_hdl, msg, websocketpp::frame::opcode::text);
            }
            catch (websocketpp::exception const& e) {
                std::cout << e.what() << std::endl;
            }
        });
    }
}


void eWebScoketCli::QueryVideoLocdState() {
    nlohmann::json jRoot;
    jRoot["ope"] = "videolock";
    jRoot["userid"] = _strUser;
    jRoot["DeviceType"] = "window";
    try {
        _cli->send(m_hdl, jRoot.dump().c_str(), websocketpp::frame::opcode::text);
    }
    catch (websocketpp::exception const& e) {
        std::cout << e.what() << std::endl;
    }
}

void eWebScoketCli::FreeVideo() {
    nlohmann::json jRoot;
    jRoot["ope"] = "videounlock";
    jRoot["userid"] = _strUser;
    jRoot["DeviceType"] = "window";
    try {
        _cli->send(m_hdl, jRoot.dump().c_str(), websocketpp::frame::opcode::text);
    }
    catch (websocketpp::exception const& e) {
        std::cout << e.what() << std::endl;
    }
}

void eWebScoketCli::loop() {
    try {
        // Set logging to be pretty verbose (everything except message payloads)
        /*_cli.set_access_channels(websocketpp::log::alevel::all);
        _cli.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize ASIO
        _cli.init_asio();

        // Register our message handler
        _cli.set_message_handler(bind(&eWebScoketCli::on_message, this, &_cli, ::_1, ::_2));
        //_cli.set_fail_handler()*/

        websocketpp::lib::error_code ec;
        client::connection_ptr con = _cli->get_connection(_strWS_uri, ec);
        if (ec) {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
            //return 0;
        }

        m_hdl = con->get_handle();
        // Note that connect here only requests a connection. No network messages are
        // exchanged until the event loop starts running in the next line.
        _cli->connect(con);

        // Start the ASIO io_service run loop
        // this will cause a single connection to be made to the server. c.run()
        // will exit when this connection is closed.
        _cli->run();
    }
    catch (websocketpp::exception const& e) {
        std::cout << e.what() << std::endl;
    }catch(...){
        std::cout << "unknow" << std::endl;
    }

    _bRun = false;
    qDebug() << "quit loop";

    if(_bDone){
        _thread_con.notify_all();
        qDebug() << "notify cloes";
    }
}
