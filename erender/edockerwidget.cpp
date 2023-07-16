#include "edockerwidget.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QResizeEvent>
#include <common/qt/eimagebtn.h>
#include <common/qt/ecolbtn.h>
#include <QDebug>
#include <base/misctool.h>
#include <common/qt/emousehover.h>


eDockerWidget::eDockerWidget(QWidget *parent) : QWidget(parent)
{

    QVBoxLayout *main_layout = new QVBoxLayout;
    QHBoxLayout *edge_layout = new QHBoxLayout;

    //手机 平板 tv 机顶盒
    eImageBtn *button1 = new eImageBtn(90, 90, "btnimg/phone_gray.png", "手机");
    eImageBtn *button2 = new eImageBtn(90, 90, "btnimg/pad_gray.png", "平板");
    eImageBtn *button3 = new eImageBtn(90, 90, "btnimg/tv_gray.png", "tv");
    eImageBtn *button4 = new eImageBtn(90, 90, "btnimg/stb_gray.png", "机顶盒");

    edge_layout->addWidget(button1);
    edge_layout->addWidget(button2);
    edge_layout->addWidget(button3);
    edge_layout->addWidget(button4);
    main_layout->addLayout(edge_layout);

    QHBoxLayout *fps_layout = new QHBoxLayout;
    QLabel *label = new QLabel("帧率");
    ecolbtn* btn_30fps = new ecolbtn(120, 30, "rgb(0,0,0)", "rgb(0,0,255)", "30");
    ecolbtn* btn_48fps = new ecolbtn(120, 30, "rgb(0,0,0)", "rgb(0,0,255)", "48");
    ecolbtn* btn_60fps = new ecolbtn(120, 30, "rgb(0,0,0)", "rgb(0,0,255)", "60");

    fps_layout->addWidget(label);
    fps_layout->addWidget(btn_30fps);
    fps_layout->addWidget(btn_48fps);
    fps_layout->addWidget(btn_60fps);
    main_layout->addLayout(fps_layout);

    QHBoxLayout *quality_layout = new QHBoxLayout;
    QLabel *quality_label = new QLabel("质量");
    //流畅  标准 高清 蓝光
    ecolbtn* btn_q1 = new ecolbtn(90, 30, "rgb(0,0,0)", "rgb(0,0,255)", "流畅");
    ecolbtn* btn_q2 = new ecolbtn(90, 30, "rgb(0,0,0)", "rgb(0,0,255)", "标准");
    ecolbtn* btn_q3 = new ecolbtn(90, 30, "rgb(0,0,0)", "rgb(0,0,255)", "高清");
    ecolbtn* btn_q4 = new ecolbtn(90, 30, "rgb(0,0,0)", "rgb(0,0,255)", "蓝光");
    quality_layout->addWidget(quality_label);
    quality_layout->addWidget(btn_q1);
    quality_layout->addWidget(btn_q2);
    quality_layout->addWidget(btn_q3);
    quality_layout->addWidget(btn_q4);
    main_layout->addLayout(quality_layout);

    QHBoxLayout *quit_layout = new QHBoxLayout;
    QPushButton *btn_quit = new QPushButton("退出云主机");

    quit_layout->addWidget(btn_quit);
    main_layout->addLayout(quit_layout);


    QHBoxLayout *mainbtn_layout = new QHBoxLayout;
    eImageBtn* btn_return = new eImageBtn(60, 60, "btnimg/return.png", "");
    eImageBtn* btn_home = new eImageBtn(60, 60, "btnimg/home.png", "");
    eImageBtn* btn_switch = new eImageBtn(60, 60, "btnimg/switch.png", "");
    mainbtn_layout->addWidget(btn_return);
    mainbtn_layout->addWidget(btn_home);
    mainbtn_layout->addWidget(btn_switch);
    main_layout->addLayout(mainbtn_layout);

    setLayout(main_layout);
    //this->setTabletTracking(true);

    emouseHover *mmHover = new emouseHover();//实例化
    connect(mmHover, &emouseHover::signal_sendBtnObj, this, &eDockerWidget::slot_MouseTrack);//连接信号槽

    this->installEventFilter(mmHover);

    _timer_id = startTimer(3000);

    connect(btn_quit, &QPushButton::clicked, this, &eDockerWidget::quit_clicked);
    connect(btn_q1, &QPushButton::clicked, this, &eDockerWidget::q1_click);
    connect(btn_q2, &QPushButton::clicked, this, &eDockerWidget::q2_click);
    connect(btn_q3, &QPushButton::clicked, this, &eDockerWidget::q3_click);
    connect(btn_q4, &QPushButton::clicked, this, &eDockerWidget::q4_click);
    connect(btn_30fps, &QPushButton::clicked, this, &eDockerWidget::fps_30_click);
    connect(btn_48fps, &QPushButton::clicked, this, &eDockerWidget::fps_48_click);
    connect(btn_60fps, &QPushButton::clicked, this, &eDockerWidget::fps_60_click);
    connect(btn_home, &QPushButton::clicked, this, &eDockerWidget::home_click);
    connect(btn_return, &QPushButton::clicked, this, &eDockerWidget::return_click);
    connect(btn_switch, &QPushButton::clicked, this, &eDockerWidget::Process_click);

    setAttribute(Qt::WA_TranslucentBackground);

}

void eDockerWidget::SetTopWindow(){
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags | Qt::WindowStaysOnTopHint);
    this->raise();
    show();
}


void eDockerWidget::mouseMoveEvent(QMouseEvent *event){
    qDebug() << "move" << QString(GetCurrentTimeStamp(1).c_str());
}

void eDockerWidget::resetTimer(){
    if(_timer_id){
        killTimer(_timer_id);
        _timer_id = 0;
    }
    _timer_id = startTimer(2000);
}

void eDockerWidget::slot_MouseTrack(QObject*, QString objName){
    if (objName == "mouse out"){
        qDebug() << "track out" << QString(GetCurrentTimeStamp(1).c_str());
        resetTimer();
        HideWindow();
    }else{
        qDebug() << "track in" << QString(GetCurrentTimeStamp(1).c_str());
        if(_timer_id){
            killTimer(_timer_id);
        }
        ShowWindow();
    }
}

void eDockerWidget::HideWindow(){
    QDesktopWidget* desktopWidget = QApplication::desktop();
    //获取可用桌面大小
    QRect deskRect = desktopWidget->availableGeometry();
    //获取设备屏幕大小
    QRect screenRect = desktopWidget->screenGeometry();
    int offset_x = screenRect.width() / 2 - this->width() / 2;
    //this->setGeometry(offset_x, 0, width(), 20);

    this->setFixedHeight(20);

}

void eDockerWidget::ShowWindow(){
    QDesktopWidget* desktopWidget = QApplication::desktop();
    //获取可用桌面大小
    QRect deskRect = desktopWidget->availableGeometry();
    //获取设备屏幕大小
    QRect screenRect = desktopWidget->screenGeometry();
    QRect rc = this->geometry();
    int h = rc.height();
    int w = rc.width();
    int offset_x = screenRect.width() / 2 - this->width() / 2;
    this->setFixedHeight(_init_h);
    this->move(offset_x, 0);
    if(_timer_id == 0){
        _timer_id = startTimer(3000);
    }
}

void eDockerWidget::resizeEvent(QResizeEvent *event){
    const QSize rc = event->size();
    if(!_bsize){
        _bsize = true;
        _init_w = rc.width();
        _init_h = rc.height();
        QWidget* shadow = new QWidget(this);
        shadow->setGeometry(0, 0, this->width(), this->height());
        QString style("QWidget{border-radius:0px; background-color:rgba(255,255,255,128);}");
        shadow->setStyleSheet(style);
        shadow->lower();
        ShowWindow();
    }
}

void eDockerWidget::timerEvent(QTimerEvent *event){
    int timerid = event->timerId();
    if(timerid == _timer_id){
        HideWindow();
        killTimer(timerid);
        _timer_id = 0;
    }
}

void eDockerWidget::quit_clicked(bool checked){
    qDebug() << "quit";
    QWidget::close();
    emit close_main_signal();
}

void eDockerWidget::fps_30_click(bool checked){
    emit fps_signal(30);
}

void eDockerWidget::fps_48_click(bool checked){
    emit fps_signal(48);
}

void eDockerWidget::fps_60_click(bool checked){
    emit fps_signal(60);
}

void eDockerWidget::q1_click(bool checked){
    emit bitrate_signal(600);
}

void eDockerWidget::q2_click(bool checked){
    emit bitrate_signal(2000);
}

void eDockerWidget::q3_click(bool checked){
    emit bitrate_signal(4000);
}

void eDockerWidget::q4_click(bool checked){
    emit bitrate_signal(6000);
}

void eDockerWidget::home_click(bool checked){
    emit home_signal();
}

void eDockerWidget::Process_click(bool checked){
    emit process_signal();
}

void eDockerWidget::return_click(bool checked){
    emit return_signal();
}
