#include "eimagebtn.h"
#include <QVBoxLayout>
#include <QColor>

eImageBtn::eImageBtn(const int w, const int h,
                     const QString& imgurl, const QString& sel_imgurl,
                     const QString &text, const QString &deftxtcol,
                     const QString &seltextcol, QWidget *parent) : QPushButton(parent)
{
    QPushButton::setFixedSize(w, h);

    buttonImage = new QLabel();

    _img_def = QString("QLabel{background-image: url(%1);"
                              "background-repeat: no-repeat; background-position: center;"
                              "background-attachment: fixed; background-attachment: fixed;"
                              "background-attachment: fixed; background-clip: padding}").arg(imgurl);

    _img_sel = QString("QLabel{background-image: url(%1);"
                              "background-repeat: no-repeat; background-position: center;"
                              "background-attachment: fixed; background-attachment: fixed;"
                              "background-attachment: fixed; background-clip: padding}").arg(sel_imgurl);

    buttonImage->setStyleSheet(_img_def);


    buttonTxt = new QLabel();
    if(text.size() == 0){
        buttonTxt->setFixedHeight(0);
    }else{
        buttonTxt->setFixedHeight(30);
    }
    buttonTxt->setText(text);
    buttonTxt->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

    _text_def = QString("QLabel{color:%1; font-size:14px;}").arg(deftxtcol);
    buttonTxt->setStyleSheet(_text_def);

    _text_sel = QString("QLabel{color:%1; font-size:14px;}").arg(seltextcol);

    QVBoxLayout *topLayout = new QVBoxLayout();
    topLayout->addWidget(buttonImage);
    topLayout->setContentsMargins(0,0,0,0);

    QVBoxLayout *bottomLayout = new QVBoxLayout();
    bottomLayout->addWidget(buttonTxt);
    bottomLayout->setMargin(0);
    bottomLayout->setSpacing(0);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);

    QPushButton::setLayout(mainLayout);

    setAttribute(Qt::WA_TranslucentBackground);
    //setStyleSheet("border-style: none;");
    QString style("QWidget{border-radius:0px; background-color:rgba(0,0,0,128);}");
    setStyleSheet(style);
}

void eImageBtn::Sel(bool sel){
    if(sel){
        buttonImage->setStyleSheet(_img_sel);
        buttonTxt->setStyleSheet(_text_sel);
    }else{
        buttonImage->setStyleSheet(_img_def);
        buttonTxt->setStyleSheet(_text_def);
    }
}
