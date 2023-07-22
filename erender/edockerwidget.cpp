#include "edockerwidget.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QDebug>
#include <base/misctool.h>
#include <common/qt/emousehover.h>
#include <common/qt/ecustomtoast.h>

eDockerWidget::eDockerWidget(QWidget *parent) : QWidget(parent)
{

    QVBoxLayout *main_layout = new QVBoxLayout;
    QHBoxLayout *edge_layout = new QHBoxLayout;

    //手机 平板 tv 机顶盒
    button1 = new eImageBtn(92, 100, "btnimg/phone_gray.png", "btnimg/phone.png",
                                       "手机", "rgb(220,220,220)", "rgb(0,114,255)");
    button2 = new eImageBtn(92, 100, "btnimg/pad_gray.png", "btnimg/pad.png",
                                       "平板", "rgb(220,220,220)", "rgb(0,114,255)");
    button3 = new eImageBtn(92, 100, "btnimg/tv_gray.png", "btnimg/tv.png",
                                       "电视", "rgb(220,220,220)", "rgb(0,114,255)");
    button4 = new eImageBtn(92, 100, "btnimg/stb_gray.png", "btnimg/stb.png",
                                       "机顶盒", "rgb(220,220,220)", "rgb(0,114,255)");

    edge_layout->addWidget(button1);
    edge_layout->addWidget(button2);
    edge_layout->addWidget(button3);
    edge_layout->addWidget(button4);
    main_layout->addLayout(edge_layout);

    QHBoxLayout *fps_layout = new QHBoxLayout;
    fps_layout->setMargin(0);
    fps_layout->setSpacing(0);

    QString def_label = QString("QLabel{color:rgb(220,220,220); font-size:14px; border-radius:0px; background-color:rgba(0,0,0,128);}");
    QLabel *label = new QLabel("帧率:");
    label->setStyleSheet(def_label);
    label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    btn_30fps = new ecolbtn(125, 30, "rgb(220,220,220)", "rgb(0,114,255)", "30");
    btn_48fps = new ecolbtn(125, 30, "rgb(220,220,220)", "rgb(0,114,255)", "48");
    btn_60fps = new ecolbtn(125, 30, "rgb(220,220,220)", "rgb(0,114,255)", "60");

    fps_layout->addWidget(label);
    fps_layout->addWidget(btn_30fps);
    fps_layout->addWidget(btn_48fps);
    fps_layout->addWidget(btn_60fps);
    main_layout->addLayout(fps_layout);

    QHBoxLayout *quality_layout = new QHBoxLayout;
    quality_layout->setSpacing(0);
    QLabel *quality_label = new QLabel("质量:");
    quality_label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    quality_label->setStyleSheet(def_label);
    //流畅  标准 高清 蓝光
    btn_q1 = new ecolbtn(94, 30, "rgb(220,220,220)", "rgb(0,114,255)", "流畅");
    btn_q2 = new ecolbtn(94, 30, "rgb(220,220,220)", "rgb(0,114,255)", "标准");
    btn_q3 = new ecolbtn(94, 30, "rgb(220,220,220)", "rgb(0,114,255)", "高清");
    btn_q4 = new ecolbtn(94, 30, "rgb(220,220,220)", "rgb(0,114,255)", "蓝光");
    quality_layout->addWidget(quality_label);
    quality_layout->addWidget(btn_q1);
    quality_layout->addWidget(btn_q2);
    quality_layout->addWidget(btn_q3);
    quality_layout->addWidget(btn_q4);
    main_layout->addLayout(quality_layout);

    QHBoxLayout *quit_layout = new QHBoxLayout;
    //QPushButton *btn_quit = new QPushButton("退出云主机");
    ecolbtn* btn_quit = new ecolbtn(420, 30, "rgb(220,220,220)", "rgb(0,114,255)", "退出云主机");

    quit_layout->addWidget(btn_quit);
    main_layout->addLayout(quit_layout);


    QHBoxLayout *mainbtn_layout = new QHBoxLayout;
    mainbtn_layout->setSpacing(0);
    eImageBtn* btn_return = new eImageBtn(60, 60, "btnimg/return.png", "", "", "", "");
    eImageBtn* btn_home = new eImageBtn(60, 60, "btnimg/home.png", "", "", "", "");
    eImageBtn* btn_switch = new eImageBtn(60, 60, "btnimg/switch.png", "", "", "", "");
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
    connect(button1, &QPushButton::clicked, this, &eDockerWidget::Mobile_click);
    connect(button2, &QPushButton::clicked, this, &eDockerWidget::Pad_click);
    connect(button3, &QPushButton::clicked, this, &eDockerWidget::Tv_click);
    connect(button4, &QPushButton::clicked, this, &eDockerWidget::Stb_click);

    setAttribute(Qt::WA_TranslucentBackground);

    btn_30fps->SetState(ecolbtn::STATE::SEL);
    btn_q1->SetState(ecolbtn::STATE::SEL);

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

    this->setFixedHeight(5);

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
        QString style("QWidget{border-radius:0px; background-color:rgba(255,255,255,100);}");
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
    CustomToast::instance().show(CustomToast::INFO, "已切换30帧");
    btn_30fps->SetState(ecolbtn::STATE::SEL);
    btn_48fps->SetState(ecolbtn::STATE::DEF);
    btn_60fps->SetState(ecolbtn::STATE::DEF);
}

void eDockerWidget::fps_48_click(bool checked){
    emit fps_signal(48);
    CustomToast::instance().show(CustomToast::INFO, "已切换48帧");
    btn_30fps->SetState(ecolbtn::STATE::DEF);
    btn_48fps->SetState(ecolbtn::STATE::SEL);
    btn_60fps->SetState(ecolbtn::STATE::DEF);
}

void eDockerWidget::fps_60_click(bool checked){
    emit fps_signal(60);
    CustomToast::instance().show(CustomToast::INFO, "已切换60帧");
    btn_30fps->SetState(ecolbtn::STATE::DEF);
    btn_48fps->SetState(ecolbtn::STATE::DEF);
    btn_60fps->SetState(ecolbtn::STATE::SEL);
}

void eDockerWidget::q1_click(bool checked){
    emit bitrate_signal(600);
    QString msg = QString("已切换-%1").arg(btn_q1->text());
    CustomToast::instance().show(CustomToast::INFO, msg);
    btn_q1->SetState(ecolbtn::STATE::SEL);
    btn_q2->SetState(ecolbtn::STATE::DEF);
    btn_q3->SetState(ecolbtn::STATE::DEF);
    btn_q4->SetState(ecolbtn::STATE::DEF);
}

void eDockerWidget::q2_click(bool checked){
    emit bitrate_signal(2000);
    QString msg = QString("已切换-%1").arg(btn_q2->text());
    CustomToast::instance().show(CustomToast::INFO, msg);
    btn_q1->SetState(ecolbtn::STATE::DEF);
    btn_q2->SetState(ecolbtn::STATE::SEL);
    btn_q3->SetState(ecolbtn::STATE::DEF);
    btn_q4->SetState(ecolbtn::STATE::DEF);
}

void eDockerWidget::q3_click(bool checked){
    emit bitrate_signal(4000);
    QString msg = QString("已切换-%1").arg(btn_q3->text());
    CustomToast::instance().show(CustomToast::INFO, msg);
    btn_q1->SetState(ecolbtn::STATE::DEF);
    btn_q2->SetState(ecolbtn::STATE::DEF);
    btn_q3->SetState(ecolbtn::STATE::SEL);
    btn_q4->SetState(ecolbtn::STATE::DEF);
}

void eDockerWidget::q4_click(bool checked){
    emit bitrate_signal(6000);
    QString msg = QString("已切换-%1").arg(btn_q4->text());
    CustomToast::instance().show(CustomToast::INFO, msg);
    btn_q1->SetState(ecolbtn::STATE::DEF);
    btn_q2->SetState(ecolbtn::STATE::DEF);
    btn_q3->SetState(ecolbtn::STATE::DEF);
    btn_q4->SetState(ecolbtn::STATE::SEL);
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

void eDockerWidget::Mobile_click(bool checked){
    emit switch_dev_signal("phone");
}

void eDockerWidget::Pad_click(bool checked){
    emit switch_dev_signal("pad");
}

void eDockerWidget::Tv_click(bool checked){
    emit switch_dev_signal("tv");
}

void eDockerWidget::Stb_click(bool checked){
    emit switch_dev_signal("box");
}

void eDockerWidget::Show_dev_slot(QString dev){
    std::string strdev = dev.toStdString();
    std::vector<std::string> devs = stringSplit(strdev, ',');
    button1->Sel(false);
    button2->Sel(false);
    button3->Sel(false);
    button4->Sel(false);
    for(auto it : devs){
        if(it.compare("phone") == 0){
            button1->Sel(true);
        }else if(it.compare("pad") == 0){
            button2->Sel(true);
        }else if(it.compare("tv") == 0){
            button3->Sel(true);
        }else if(it.compare("box") == 0){
            button4->Sel(true);
        }
    }
}
