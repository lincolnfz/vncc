#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qprocess.h>
#include <qdialog.h>
#include <QDebug>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <unistd.h>
#include "base/callback/callback.h"
#include "playdialog.h"
#include "base/misctool.h"
#include "eWebRequest.h"
#include "common/eInstructions.h"
#include "common/defThread.h"
#include <qwindow.h>
#include <QDesktopWidget>
#include <QMouseEvent>
#include "common/qt/ecustomtoast.h"
#include "erpcmanager.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    _misc_thd = std::make_shared<MiscThrad>();
    nbase::ThreadManager::RegisterThread((int)ThreadId::kThreadGlobalMisc , _misc_thd);
    _misc_thd->Start();

    ui->setupUi(this);

    nbase::StdClosure fn = ToWeakCallback(nbase::Bind([]()->void{
            qDebug("hello\n");
        }));
    //nbase::ThreadManager::PostRepeatedTask(1, fn, nbase::TimeDelta::FromSeconds(1), -1);

    int l, s;
    nbase::StdClosure fn2 = ToWeakCallback(nbase::Bind([=](int l, int s)->void {

        }, l, s));
    nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl, fn2);

    move(qApp->desktop()->availableGeometry(this).center()- this->rect().center());


    _settings = new QSettings ("config.ini", QSettings::IniFormat);

    QVariant user = _settings->value("user");

    QVariant pwd = _settings->value("pwd");

    QVariant vncip = _settings->value("vncip");

    ui->lineEdit->setText(user.toString());
    ui->pwd_edit->setText(pwd.toString());
    ui->pwd_edit->setEchoMode(QLineEdit::Password);
    sRpcManager.GenerateRpcLayout();

    ui->edit_vnc_addr->setText(vncip.toString());

    QPixmap pixmap("btnimg/demo_bk.png");
    QPalette palette;
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    connect(&sWebsocket, &eWebScoketCli::ShowBtns_signal, this, &MainWindow::slot_show_btns);
    connect(&sWebsocket, &eWebScoketCli::Launch_signal, this, &MainWindow::slot_show);

    setWindowFlags( Qt::FramelessWindowHint);
    //setFixedSize(743, 580);
    //QStatusBar *statusBar = new QStatusBar();
    //statusBar->setSizeGripEnabled(false); // 将状态栏的大小设置为0
    //setStatusBar(statusBar);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_conn_clicked()
{
    //test_widget();
    {
        std::string str_user, str_pwd;
        str_user = "13600000003";
        str_pwd = "a12345678";
        sWebReq.RHLogin(str_user.c_str(), str_pwd.c_str(),
                nullptr, this->GetWeakFlag(),
                [=](std::shared_ptr<eWebRequest::eWebExchangeData> sp, void*)->void {
                    if (sp->isSucc()) {
                        _RHLOGINRET rhlogin;
                        QuickGetJsonData(sp->_data.c_str(), rhlogin);
                        char va[128] = {0};
                        sprintf(va, "%s:%d", rhlogin.DeviceData.LocalIP.c_str(), rhlogin.DeviceData.AVPort);
                        char remote[128] = {0};
                        sprintf(remote, "%s:%d", rhlogin.DeviceData.IP.c_str(), rhlogin.DeviceData.AdbPort + 512);
                        char cmd[128] = {0};
                        sprintf(cmd, "%s:%d", rhlogin.DeviceData.IP.c_str(), rhlogin.DeviceData.AdbPort + 256);
                        char ws[128] = {0};
                        sprintf(ws, "%s:%d", rhlogin.DeviceData.IP.c_str(), rhlogin.DeviceData.WebRTCPort);
                        char stun[128] = {0};
                        sprintf(stun, "%s:%d", rhlogin.DeviceData.IP.c_str(), 3478);
                        qDebug("va:%s,remote:%s,cmd:%s,ws:%s,stun:%s", va, remote, cmd, ws, stun);

                    }
                    else {
                        nbase::StdClosure fn = nbase::Bind(&MainWindow::test_callback, this, "bad", 0);
                        nbase::ThreadManager::PostTask((int)ThreadId::kThreadUI, fn);
                    }
                });
        return;
    }
    {
        //test code
        nbase::StdClosure fn = ToWeakCallback(nbase::Bind([=]()->void{
            //test_widget();
        }));
        fn.t_();
        return;
    }
    return;
    QString vnc_adds;// = this->ui->edit_adds->text();
    QString app_path = QCoreApplication::applicationDirPath();
    std::string str_app_path = app_path.toStdString();
    char sz[512] = "xtigervncviewer";
    //sprintf(sz, "%s/SDLvncviewer", str_app_path.c_str());
    QString app_file = QString::fromStdString(sz);
    QStringList arguments;
    arguments << vnc_adds;
    arguments << "--FullScreen";
    arguments << "--normalcursor";
    QProcess proces(this);
    proces.startDetached(app_file, arguments);
    //proces.waitForFinished();
}

void MainWindow::test_widget(){
    PlayDialog* agl = new PlayDialog(this, Qt::Window);
    agl->setFixedWidth(800);
    agl->setFixedHeight(600);
    agl->exec();
    agl->window()->winId();
    delete agl;
}

void MainWindow::test_callback(const std::string& c, int idx){
    qDebug("test_callback %s", c.c_str());
}

void MainWindow::on_btn_login_clicked()
{
    action_Login();
}

void MainWindow::on_lineEdit_returnPressed()
{
    ui->pwd_edit->setFocus();
}

void MainWindow::on_pwd_edit_returnPressed()
{
    action_Login();
}

void MainWindow::on_btn_login_pressed()
{
    action_Login();
}

void MainWindow::action_Login(){
    QString pwd = ui->pwd_edit->text();
    QString acc = ui->lineEdit->text();
    std::string str_user, str_pwd;
    str_user = acc.toStdString();
    str_pwd = pwd.toStdString();

    sWebReq.RHLogin(str_user.c_str(), str_pwd.c_str(),
        nullptr, this->GetWeakFlag(),
        [=](std::shared_ptr<eWebRequest::eWebExchangeData> sp, void*)->void {
            if (sp->isSucc()) {
                sWebsocket.Stop();

                _RHLOGINRET rhlogin;
                QuickGetJsonData(sp->_data.c_str(), rhlogin);
                _avport = rhlogin.DeviceData.AVPort;
                _webrtcport = rhlogin.DeviceData.WebRTCPort;
                _adbport = rhlogin.DeviceData.AdbPort;
                _ip = rhlogin.DeviceData.IP;
                _localp_ip = rhlogin.DeviceData.LocalIP;
                char va[128] = {0};
                sprintf(va, "%s:%d", rhlogin.DeviceData.LocalIP.c_str(), rhlogin.DeviceData.AVPort);
                char remote[128] = {0};
                sprintf(remote, "%s:%d", rhlogin.DeviceData.IP.c_str(), rhlogin.DeviceData.AdbPort + 512);
                char cmd[128] = {0};
                sprintf(cmd, "%s:%d", rhlogin.DeviceData.IP.c_str(), rhlogin.DeviceData.AdbPort + 256);
                char ws[128] = {0};
                sprintf(ws, "%s:%d", rhlogin.DeviceData.IP.c_str(), rhlogin.DeviceData.WebRTCPort);
                char stun[128] = {0};
                sprintf(stun, "%s:%d", rhlogin.DeviceData.IP.c_str(), 3478);
                qDebug("va:%s,remote:%s,cmd:%s,ws:%s,stun:%s", va, remote, cmd, ws, stun);
                ui->rh_group->setCurrentIndex(1);

                //QString pwd = ui->pwd_edit->text();
                //QString acc = ui->lineEdit->text();
                _settings->setValue("user", acc);
                _settings->setValue("pwd", pwd);
                char szws[128] = {0};
                sprintf(szws, "ws://%s:%d", rhlogin.WebSocketData.Ip.c_str(), rhlogin.WebSocketData.Port);
                sWebsocket.Run(szws, str_user.c_str());
            }
            else {
                const char* utf8 = sp->_Msg.c_str();
                CustomToast::instance().show(CustomToast::WARN, QString(sp->_Msg.c_str()));
            }
        });
}

void MainWindow::on_btn_back_clicked()
{
    sWebsocket.Stop();
    ui->rh_group->setCurrentIndex(0);
}

void MainWindow::on_btn_launch_clicked()
{
    RunPlayer();
}

void MainWindow::RunPlayer(){
    ++_id;
    QString app_path = QCoreApplication::applicationDirPath();
    std::string str_app_path = app_path.toStdString();
    char sz[512] = {0};
    sprintf(sz, "%s/erender", str_app_path.c_str());
    QString app_file = QString::fromStdString(sz);
    QStringList arguments;
    nlohmann::json root;
    root["port"] = sRpcManager.port();
    root["keyid"] = _id;
    root["adb"] = _adbport;
    root["avport"] = _avport;
    root["webrtc"] = _webrtcport;
    root["ip"] = _ip;
    root["lcoal_ip"] = _localp_ip;

    arguments << QString::fromStdString(root.dump());
    QProcess proces(this);
    proces.startDetached(app_file, arguments);
}

void MainWindow::on_btn_vnc_login_clicked()
{
    QString vnc_adds = this->ui->edit_vnc_addr->text();
    if(vnc_adds.length() == 0){
        return;
    }
    _settings->setValue("vncip", vnc_adds);
    QString app_path = QCoreApplication::applicationDirPath();
    std::string str_app_path = app_path.toStdString();
    char sz[512] = "xtigervncviewer";
    //sprintf(sz, "%s/SDLvncviewer", str_app_path.c_str());
    QString app_file = QString::fromStdString(sz);
    QStringList arguments;
    arguments << vnc_adds;
    arguments << "--FullScreen";
    arguments << "--DotWhenNoCursor=1";
    //arguments << "--normalcursor";
    QProcess proces(this);
    proces.startDetached(app_file, arguments);
}

void MainWindow::slot_show_btns(QString qstr_btns){
    qDebug() << "mainwindow:" << qstr_btns;
    _qstr_btns = qstr_btns;
}

void MainWindow::slot_show(){
    RunPlayer();
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton)
    {
        clickPos.setX(event->pos().x());
        clickPos.setY(event->pos().y());
    }
    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    QMainWindow::mouseReleaseEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons() == Qt::LeftButton)
    {
        this->move(this->pos()+event->pos()-this->clickPos);
    }
    QMainWindow::mouseMoveEvent(event);
}
