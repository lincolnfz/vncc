#include "erpcmanager.h"
#include "base/misctool.h"
#include "eWebScoketCli.h"
#include <QDebug>

eRpcManager* eRpcManager::g_inst = nullptr;

eRpcManager::eRpcManager()
{
    g_inst = this;
    connect(this, &eRpcManager::signal_help_rpc_connect, this,  &eRpcManager::slot_help_rpc_connect);
    connect(this, &eRpcManager::signal_help_rcp_goto_device, this,  &eRpcManager::slot_help_rcp_goto_device);
}

eRpcManager::~eRpcManager(){

}

void eRpcManager::GenerateRpcLayout(){
    while (true) {
        if (!testTcpPortUsed(_port)) {
            try {
                _rpc_srv.reset(new rpc::server(_port));
                _rpc_srv->bind("connect_rpc", &eRpcManager::rpc_connect_handle);
                _rpc_srv->bind("goto_device", &eRpcManager::rpc_goto_device_handle);
                _rpc_srv->async_run();
                break;
            }
            catch (...) {
                //assert(false);
            }

        }
        ++_port;
        if (_port == 0) {
            break;
        }
    }
}

void eRpcManager::ShowBtns(std::string clients){
    auto got = _rpc_clis.rbegin();
    if(got != _rpc_clis.rend() ){
        try {
            got->second->async_call("active_device", clients);
        } catch (...) {
            qDebug() << "showbtns error";
        }
    }
}

void eRpcManager::rpc_connect_handle(int id, int port){
    qDebug() << "rpc_connect_handle";
    emit sRpcManager.signal_help_rpc_connect(id, port);
}

void eRpcManager::rpc_goto_device_handle(int id, std::string device){
    qDebug() << "rpc_goto_device_handle:" << QString(device.c_str());
    emit sRpcManager.signal_help_rcp_goto_device(id, QString(device.c_str()));
}

void eRpcManager::slot_help_rpc_connect(int id, short port){
    auto got = _rpc_clis.find(id);
    if(got == _rpc_clis.end()){
        _rpc_clis.insert(std::make_pair(id, std::make_unique<rpc::client>("127.0.0.1", port)));
    }
}

void eRpcManager::slot_help_rcp_goto_device(int id, QString device){
    qDebug() << "slot_help_rcp_goto_device:" << device;
    sWebsocket.SwitchDevice(device.toStdString());
}
