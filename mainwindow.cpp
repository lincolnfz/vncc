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


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    _misc_thd = std::make_shared<MiscThrad>();
    nbase::ThreadManager::RegisterThread((int)ThreadId::kThreadUI, _misc_thd);

    ui->setupUi(this);
    GenerateRpcLayout();

    nbase::StdClosure fn = ToWeakCallback(nbase::Bind([]()->void{
            qDebug("hello\n");
        }));
    nbase::ThreadManager::PostRepeatedTask(1, fn, nbase::TimeDelta::FromSeconds(1), -1);
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
    {
        std::string str_user, str_pwd;
        str_user = "13600000005";
        str_pwd = "a12345678";
        sWebReq.RHLogin(str_user.c_str(), str_pwd.c_str(),
                nullptr, this->GetWeakFlag(),
                [=](std::shared_ptr<eWebRequest::eWebExchangeData> sp, void*)->void {
                    if (sp->isSucc()) {
                        _RHLOGINRET rhlogin;
                        QuickGetJsonData(sp->_data.c_str(), rhlogin);
                        int i = 0;
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
            test_widget();
        }));
        fn.t_();
        return;
    }
    QString vnc_adds = this->ui->edit_adds->text();
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
