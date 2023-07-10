#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qevent.h>

//172.27.11.193:17513,cmd:172.27.11.193:17257

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->resize( QSize( 900, 600 ));
    installEventFilter(this);
    this->setAttribute(Qt::WA_InputMethodEnabled, true);
    this->setAttribute(Qt::WA_KeyCompression, true);
    this->setFocusPolicy(Qt::WheelFocus);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event){
    if(QEvent::InputMethod == event->type()) {
        //qDebug() << "============InputMethod====="<< hasFocus() <<"=============";
        if(!hasFocus()) //经验证，失去焦点时，输入法事件优先于FocusOut事件被处理
        {
            QInputMethodEvent* iEvent = dynamic_cast<QInputMethodEvent*> (event); //注意用将_event括起来
            iEvent->setCommitString("");    //将失去焦点后提交的字符串设置为空
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::resizeEvent(QResizeEvent* ev){
    const QSize rc = ev->size();
    if(_trans == nullptr){
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
        sprintf(ws, "ws://%s:%d", ip.c_str(), WebRTCPort);
        char stun[128] = {0};
        sprintf(stun, "stun:%s:%d", ip.c_str(), 3478);
        _trans = std::make_shared<eControlTrans>((enum ThreadId)0, "", "",
                                                 (const char*)va,
                                                 (const char*)remote,
                                                 (const char*)cmd,
                                                 (const char*)ws,
                                                 (const char*)stun, 0, 1, 0, 0);
        nbase::ThreadManager::RegisterThread((int)ThreadId::kThreadRemoteControl, _trans);
        _trans->SetWindow(this->winId(), rc.width(), rc.height());
        _trans->Start();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    QSize rc = this->size();

}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    QSize rc = this->size();
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event){
    QSize rc = this->size();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){

}

void MainWindow::wheelEvent(QWheelEvent *event){

}

/*QVariant MainWindow::inputMethodQuery(Qt::InputMethodQuery query) const{
    QMainWindow::inputMethodQuery(query);
}*/

void MainWindow::inputMethodEvent(QInputMethodEvent *event){
    QString str = event->commitString();
    qDebug("inputMethodEvent");
}

