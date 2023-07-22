#include "mainwindow.h"

#include <QApplication>
#include <qevent.h>
#include <qlogging.h>
#include <qobject.h>
#include <json.hpp>

int adbport, webrtcport, avport;
std::string ip, local_ip;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int keyid = 0;
    int port = 0;

    char* param = argv[1];
    nlohmann::json root;
    if(param){
        root = nlohmann::json::parse(param);
        keyid = root["keyid"];
        port = root["port"];
        adbport = root["adb"];
        avport = root["avport"];
        webrtcport = root["webrtc"];
        ip = root["ip"];
        local_ip = root["lcoal_ip"];
    }
    MainWindow w(keyid, port);


    w.setWindowFlags(w.windowFlags() | Qt::FramelessWindowHint);
    w.show();
    return a.exec();
}
