#ifndef ERPCMANAGER_H
#define ERPCMANAGER_H
#include <QObject>
#include <base/base.h>
#include <rpc/server.h>
#include <rpc/client.h>
#include <boost/serialization/singleton.hpp>

class eRpcManager : public QObject, public boost::serialization::singleton<eRpcManager>
{
    Q_OBJECT;
public:
    eRpcManager();
    virtual ~eRpcManager();
    unsigned short port() const {
        return _port;
    }
    void GenerateRpcLayout();
    void ShowBtns(std::string clients);

signals:
    void signal_help_rpc_connect(int id, short port);
    void signal_help_rcp_goto_device(int id, QString device);

protected slots:
    void slot_help_rpc_connect(int id, short port);
    void slot_help_rcp_goto_device(int id, QString device);


protected:

    static void rpc_connect_handle(int id, int port);
    static void rpc_goto_device_handle(int id, std::string device);

private:
    std::unique_ptr<rpc::server> _rpc_srv;
    std::map<unsigned int, std::unique_ptr<rpc::client>> _rpc_clis;
    unsigned short _port = 26186;

    static eRpcManager* g_inst;
};

typedef boost::serialization::singleton<eRpcManager> singleton_RpcManager; // Ê¹ÓÃÄ£°åµÄ·½Ê½Ö»ÔÊÐíµ¥¸öÊµÀý
#define sRpcManager singleton_RpcManager::get_mutable_instance() // ·ÇconstÊµÀý
#define sRpcManager_const singleton_RpcManager::get_const_instance() // constÊµÀý

#endif // ERPCMANAGER_H
