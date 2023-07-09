#include "mainwindow.h"
#include "ui_mainwindow.h"


//172.27.11.193:17513,cmd:172.27.11.193:17257

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int avport = 17001;
    int adbport = 15001;
    int WebRTCPort = 25002;
    std::string ip = "172.27.11.193";
    std::string local_ip = "172.27.11.193";
    char va[128] = {0};
    sprintf(va, "%s:%d", local_ip.c_str(), avport);
    char remote[128] = {0};
    sprintf(remote, "%s:%d", ip.c_str(), adbport+ 512);
    char cmd[128] = {0};
    sprintf(cmd, "%s:%d", ip.c_str(), adbport+ 256);
    char ws[128] = {0};
    sprintf(ws, "%s:%d", ip.c_str(), WebRTCPort);
    char stun[128] = {0};
    sprintf(stun, "%s:%d", ip.c_str(), 3478);
    _trans = std::make_shared<eControlTrans>((enum ThreadId)0, "", "",
                                             (const char*)va,
                                             (const char*)remote, (const char*)cmd,
                                             (const char*)ws,
                                             (const char*)stun, 0, 1, 0, 1);
    nbase::ThreadManager::RegisterThread((int)ThreadId::kThreadRemoteControl, _trans);
    _trans->Start();
    //nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl, )
}

MainWindow::~MainWindow()
{
    delete ui;
}

