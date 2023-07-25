#include "ecolbtn.h"

ecolbtn::ecolbtn(const int w, const int h,
                 const QString& defcol, const QString& selcol, const QString &text, QWidget *parent):QPushButton(text)
{
    _def_style = QString("QPushButton{color:%1; font-size:14px; border-radius:0px; background-color:rgba(0,0,0,128);}").arg(defcol);
    _sel_style = QString("QPushButton{color:%1; font-size:14px; border-radius:0px; background-color:rgba(0,0,0,128);}").arg(selcol);
    this->setFixedSize(w, h);
    this->setStyleSheet(_def_style);
}

void ecolbtn::SetState(const STATE s){
    if(s == STATE::DEF){
        this->setStyleSheet(_def_style);
    }else{
        this->setStyleSheet(_sel_style);
    }
}
