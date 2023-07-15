#include "erpcmanager.h"
#include "base/misctool.h"

eRpcManager::eRpcManager()
{

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

void eRpcManager::rpc_connect_handle(int id, unsigned short port){
    emit sRpcManager.signal_help_rpc_connect(id, port);
}

void eRpcManager::rpc_goto_device_handle(int id, std::string device){
    emit sRpcManager.signal_help_rcp_goto_device(id, device);
}

void eRpcManager::slot_help_rpc_connect(int id, short port){
    auto got = _rpc_clis.find(id);
    if(got == _rpc_clis.end()){
        _rpc_clis.insert(std::make_pair(id, std::make_unique<rpc::client>(port)));
    }
}

void eRpcManager::slot_help_rcp_goto_device(int id, std::string device){

}
