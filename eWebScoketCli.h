#pragma once

// base header
#include "base/base.h"
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <iostream>
//#include <boost/serialization/singleton.hpp>

typedef websocketpp::client<websocketpp::config::asio_client> client;
// pull out the type of messages sent by our config
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

class eWebScoketCli : public virtual nbase::SupportWeakCallback//, public boost::serialization::singleton<eWebScoketCli>
{
public:
	enum class DEVICE {
		PHONE,
		PAD,
		TV,
		BOX,
	};
	eWebScoketCli();
	virtual ~eWebScoketCli();

	bool Run(const char* ws, const char* user);
	void Stop();
	void LoginWindow();
	void SwitchDevice(DEVICE device);
	void QueryVideoLocdState();
	void FreeVideo();
    //static eWebScoketCli* g_inst;
    void SafeSend(std::string msg);

protected:
	void loop();
	void on_open(websocketpp::connection_hdl hdl);
	void on_close(websocketpp::connection_hdl hdl);
	void on_fail(websocketpp::connection_hdl hdl);
	bool on_validate(websocketpp::connection_hdl hdl);
	void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg);
	void on_heart();
	void ReConnect();

private:
	std::unique_ptr<client> _cli;
	std::string _strWS_uri;
	websocketpp::connection_hdl m_hdl;
	std::string _strUser;
	bool _bDone = false;
	bool _bRun = false;
    bool _bStopDone = false;
    std::mutex _clock_lock;
    std::condition_variable _thread_con;
};

// ʹ��typedef�Լ�������ʹ��
//typedef boost::serialization::singleton<eWebScoketCli> singleton_Websocket; // ʹ��ģ��ķ�ʽֻ������ʵ��
//#define sWebsocket singleton_Websocket::get_mutable_instance() // ��constʵ��
//#define sWebsocket_const singleton_Websocket::get_const_instance() // constʵ��
