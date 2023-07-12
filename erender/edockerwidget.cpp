#include "edockerwidget.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <common/qt/eimagebtn.h>
#include <common/qt/ecolbtn.h>

eDockerWidget::eDockerWidget(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    QWidget* shadow = new QWidget(this);
    shadow->setGeometry(0, 0, this->width(), this->height());
    QString style("QWidget{border-radius:0px; background-color:rgba(255,255,255,.5);}");
    shadow->setStyleSheet(style);

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

    setLayout(main_layout);
}
