#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qevent.h>
#include <QDebug>
#include <QHBoxLayout>

//172.27.11.193:17513,cmd:172.27.11.193:17257

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    int ss = sizeof(wchar_t);
    ui->setupUi(this);
    this->resize( QSize( 900, 600 ));
    //installEventFilter(this);
    this->setAttribute(Qt::WA_InputMethodEnabled, true);
    this->setAttribute(Qt::WA_KeyCompression, true);
    this->setFocusPolicy(Qt::WheelFocus);

    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, Qt::black);
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    //QHBoxLayout *layout = new QHBoxLayout;
    //eDockerWidget* _docker = new eDockerWidget();
    _docker = new eDockerWidget();
    //_docker->resize(90, 60);
    _docker->setWindowFlags(Qt::FramelessWindowHint);
    _docker->show();
    //this->setCentralWidget(_docker);

    //layout->addWidget(_docker);
    //setCentralWidget(new QWidget);
    //centralWidget()->setLayout(layout);

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
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
*/

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
    }else{
        _trans->DoRoate(rc.width(), rc.height(), 0.f);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    QSize rc = this->size();
    float norm_x = (float)event->x() / (float)rc.width();
    float norm_y = (float)event->y() / (float)rc.height();
    switch (event->button()) {
        case Qt::LeftButton:
            {
                  OutMouseMsg(norm_x, norm_y, 0);
            }
            break;
        case Qt::RightButton:
            {
                 //鼠标右键按下处理
                RButtonEvent(norm_x, norm_y, 1);
            }
            break;
        default:
            break;
    }

    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    QSize rc = this->size();
    float norm_x = (float)event->x() / (float)rc.width();
    float norm_y = (float)event->y() / (float)rc.height();
    switch (event->button()) {
        case Qt::LeftButton:
            {
                OutMouseMsg(norm_x, norm_y, 1);
            }
            break;
        case Qt::RightButton:
            {
                RButtonEvent(norm_x, norm_y, 2);
            }
            break;
        default:
            break;
    }
    QMainWindow::mouseReleaseEvent(event);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event){
    QSize rc = this->size();
    float norm_x = (float)event->x() / (float)rc.width();
    float norm_y = (float)event->y() / (float)rc.height();
    switch (event->button()) {
        case Qt::LeftButton:
            {

            }
            break;
        case Qt::RightButton:
            {

            }
            break;
        default:
            break;
    }
    QMainWindow::mouseDoubleClickEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    QSize rc = this->size();
    float norm_x = (float)event->x() / (float)rc.width();
    float norm_y = (float)event->y() / (float)rc.height();
    Qt::MouseEventFlags flag = event->flags();
    Qt::MouseButtons btns = event->buttons();
    if (Qt::LeftButton == (btns & Qt::LeftButton) ){
        //drag mouse move
        OutMouseMsg(norm_x, norm_y, 2);
    }
    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::wheelEvent(QWheelEvent *event){
    QSize rc = this->size();
    float norm_x = (float)event->x() / (float)rc.width();
    float norm_y = (float)event->y() / (float)rc.height();
    int zDelta = event->delta();
    Qt::KeyboardModifiers modi = event->modifiers();
    bool bshift = false;
    bool balt = false;
    bool bctrl = false;
    bshift = !!(modi & Qt::KeyboardModifier::ShiftModifier);
    balt = !!(modi & Qt::KeyboardModifier::AltModifier);
    bctrl = !!(modi & Qt::KeyboardModifier::ControlModifier);
    if(bctrl){
        //view zoom
    }else{
        OutMouseMsg(norm_x, norm_y, 42);
        OutMouseMsg(norm_x, norm_y, (zDelta >= 0) ? 44 : 43);
    }

    QMainWindow::wheelEvent(event);
}

/*QVariant MainWindow::inputMethodQuery(Qt::InputMethodQuery query) const{
    QMainWindow::inputMethodQuery(query);
}*/

void MainWindow::inputMethodEvent(QInputMethodEvent *event){
    QString str = event->commitString();
    qDebug() << str;
    OutTextMsg(str);
    QMainWindow::inputMethodEvent(event);
}

void MainWindow::focusInEvent(QFocusEvent *event){
    QMainWindow::focusInEvent(event);
}

void MainWindow::focusOutEvent(QFocusEvent *event){
    QMainWindow::focusOutEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    //qDebug() << "press" << event->text();
    Qt::KeyboardModifiers modi = event->modifiers();
    bool bshift = false;
    bool balt = false;
    bool bctrl = false;
    bshift = !!(modi & Qt::KeyboardModifier::ShiftModifier);
    balt = !!(modi & Qt::KeyboardModifier::AltModifier);
    bctrl = !!(modi & Qt::KeyboardModifier::ControlModifier);
    //qDebug() << "shift:" << bshift << " alt:" << balt << " ctrl:" << bctrl << " txt:" << event->text();
    if(!bctrl && !bshift && !balt){
        QString txt = event->text();
        OutTextMsg(txt);
    }
    if(bctrl){
        //qDebug() << "key:" << event->key();
        CtrlAccelrate(event->key());
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event){
    //qDebug() << "release" << event->text();

    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event){
    emit _docker->close();
    event->accept();
}

void MainWindow::OutMouseMsg(float x, float y, int mtype){
    _trans->SendMouseMsg(x, y, mtype);
}

void MainWindow::OutTextMsg(QString& txt){
    std::wstring wtxt = txt.toStdWString();
    std::string str_txt = nbase::UTF16ToUTF8(wtxt);
    if(str_txt.empty() == false){
        qDebug() << "text size:" << str_txt.length();
        _trans->SendTextMsg(str_txt);
    }
}

void MainWindow::CtrlAccelrate(int nKeyCode){
    if (nKeyCode == 86) { //86
        std::string szcmd = "sendevent $(ls /dev/input/event*) 1 29 1 && sendevent $(ls /dev/input/event*) 0 0 0 && sendevent $(ls /dev/input/event*) 1 47 1 && sendevent $(ls /dev/input/event*) 0 0 0 && sendevent $(ls /dev/input/event*) 1 47 0 && sendevent $(ls /dev/input/event*) 0 0 0 && sendevent $(ls /dev/input/event*) 1 29 0 && sendevent $(ls /dev/input/event*) 0 0 0";
        _trans->SendLogEvent(szcmd);
    }
    else if (nKeyCode == 65) { //65
        std::string szcmd = "sendevent $(ls /dev/input/event*) 1 29 1 && sendevent $(ls /dev/input/event*) 0 0 0 && sendevent $(ls /dev/input/event*) 1 30 1 && sendevent $(ls /dev/input/event*) 0 0 0 && sendevent $(ls /dev/input/event*) 1 30 0 && sendevent $(ls /dev/input/event*) 0 0 0 && sendevent $(ls /dev/input/event*) 1 29 0 && sendevent $(ls /dev/input/event*) 0 0 0";
        _trans->SendLogEvent(szcmd);
    }
    else if (nKeyCode == 67) { //67
        std::string szcmd = "sendevent $(ls /dev/input/event*) 1 29 1 && sendevent $(ls /dev/input/event*) 0 0 0 && sendevent $(ls /dev/input/event*) 1 46 1 && sendevent $(ls /dev/input/event*) 0 0 0 && sendevent $(ls /dev/input/event*) 1 46 0 && sendevent $(ls /dev/input/event*) 0 0 0 && sendevent $(ls /dev/input/event*) 1 29 0 && sendevent $(ls /dev/input/event*) 0 0 0";
        _trans->SendLogEvent(szcmd);
    }
    else if (nKeyCode == 88) { //88
        std::string szcmd = "sendevent $(ls /dev/input/event*) 1 29 1 && sendevent $(ls /dev/input/event*) 0 0 0 && sendevent $(ls /dev/input/event*) 1 45 1 && sendevent $(ls /dev/input/event*) 0 0 0 && sendevent $(ls /dev/input/event*) 1 45 0 && sendevent $(ls /dev/input/event*) 0 0 0 && sendevent $(ls /dev/input/event*) 1 29 0 && sendevent $(ls /dev/input/event*) 0 0 0";
        _trans->SendLogEvent(szcmd);
    }
    else if (nKeyCode == 83) {//83
        std::string szcmd = "sendevent $(ls /dev/input/event*) 1 29 1 && sendevent $(ls /dev/input/event*) 0 0 0 && sendevent $(ls /dev/input/event*) 1 31 1 && sendevent $(ls /dev/input/event*) 0 0 0 && sendevent $(ls /dev/input/event*) 1 31 0 && sendevent $(ls /dev/input/event*) 0 0 0 && sendevent $(ls /dev/input/event*) 1 29 0 && sendevent $(ls /dev/input/event*) 0 0 0";
        _trans->SendLogEvent(szcmd);
    }
    else if (nKeyCode == 90) {//90
        std::string szcmd = "sendevent $(ls /dev/input/event*) 1 29 1 && sendevent $(ls /dev/input/event*) 0 0 0 && sendevent $(ls /dev/input/event*) 1 44 1 && sendevent $(ls /dev/input/event*) 0 0 0 && sendevent $(ls /dev/input/event*) 1 44 0 && sendevent $(ls /dev/input/event*) 0 0 0 && sendevent $(ls /dev/input/event*) 1 29 0 && sendevent $(ls /dev/input/event*) 0 0 0";
        _trans->SendLogEvent(szcmd);
    }
    else if (nKeyCode == 66) {//66
        std::string szcmd = "sendevent $(ls /dev/input/event*) 1 29 1 && sendevent $(ls /dev/input/event*) 0 0 0 && sendevent $(ls /dev/input/event*) 1 48 1 && sendevent $(ls /dev/input/event*) 0 0 0 && sendevent $(ls /dev/input/event*) 1 48 0 && sendevent $(ls /dev/input/event*) 0 0 0 && sendevent $(ls /dev/input/event*) 1 29 0 && sendevent $(ls /dev/input/event*) 0 0 0";
        _trans->SendLogEvent(szcmd);
    }
}

void MainWindow::RButtonEvent(float x, float y, int mtype){
    _trans->RButtonEvent(x, y, mtype);
}

