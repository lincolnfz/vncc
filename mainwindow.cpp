#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qprocess.h>
#include <qdialog.h>
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
#include "common/qt/ecustomtoast.h"
#include <QSettings>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    _misc_thd = std::make_shared<MiscThrad>();
    nbase::ThreadManager::RegisterThread((int)ThreadId::kThreadGL, _misc_thd);
    _misc_thd->Start();

    ui->setupUi(this);
    GenerateRpcLayout();

    nbase::StdClosure fn = ToWeakCallback(nbase::Bind([]()->void{
            qDebug("hello\n");
        }));
    //nbase::ThreadManager::PostRepeatedTask(1, fn, nbase::TimeDelta::FromSeconds(1), -1);

    int l, s;
    nbase::StdClosure fn2 = ToWeakCallback(nbase::Bind([=](int l, int s)->void {

        }, l, s));
    nbase::ThreadManager::PostTask((int)ThreadId::kThreadRemoteControl, fn2);

    move(qApp->desktop()->availableGeometry(this).center()- this->rect().center());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::GenerateRpcLayout(){
    while (true) {
        if (!testTcpPortUsed(_port)) {
            try {
                _srv.reset(new rpc::server(_port));
                _srv->async_run();
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
    char sz[512] = {0};
    sprintf(sz, "%s/SDLvncviewer", str_app_path.c_str());
    QString app_file = QString::fromStdString(sz);
    QStringList arguments;
    arguments << vnc_adds;
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
                ui->rh_group->setCurrentIndex(1);
            }
            else {
                const char* utf8 = sp->_Msg.c_str();
                CustomToast::instance().show(CustomToast::WARN, QString(sp->_Msg.c_str()));
            }
        });
}
