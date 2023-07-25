#include "emousehover.h"
#include <QEvent>

emouseHover::emouseHover(QObject *parent) : QObject(parent)
{

}

bool emouseHover::eventFilter(QObject *obj, QEvent *event){
    if (event->type() == QEvent::Enter){//鼠标进入控件
       emit signal_sendBtnObj(obj, obj->objectName());
       return true;
   }else if (event->type() == QEvent::Leave){//鼠标离开控件
       emit signal_sendBtnObj(obj, "mouse out");
       return true;
   }
   return  QObject::eventFilter(obj, event);
}
