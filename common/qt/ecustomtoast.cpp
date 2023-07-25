#include "ecustomtoast.h"
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QDebug>

class ToastDailog: public QDialog
{
public:
    ToastDailog()
    {
        auto layout = new QHBoxLayout;//水平布局
        m_label = new QLabel;
        m_label->setStyleSheet("color:white; background:transparent");
        layout->addWidget(m_label);
        m_label->setAlignment(Qt::AlignCenter); //文字居中
        setLayout(layout);
        setWindowFlag(Qt::FramelessWindowHint); //隐藏菜单框
        setAttribute(Qt::WA_ShowWithoutActivating, true);
        QWidget* shadow = new QWidget(this);
        shadow->setGeometry(0, 0, this->width(), this->height());
        QString style("QWidget{border-radius:0px; background-color:rgba(255,255,255,128);}");
        shadow->setStyleSheet(style);
    }

    void show(CustomToast::Level level, const QString &text)
    {
        QPalette p = palette();
        p.setColor(QPalette::Window, QColor(0, 0, 0, 200));
        switch (level) {
        case CustomToast::INFO:
            p.setColor(QPalette::Window, QColor(0, 0, 0, 255));//黑底
            break;
        case CustomToast::WARN:
            p.setColor(QPalette::Window, QColor(0, 0, 255, 255));//蓝底
            break;
        default:
            p.setColor(QPalette::Window, QColor(255, 0, 0, 255));//红底
            break;

        }

        setPalette(p);
        m_label->setText(text);
        setWindowFlag(Qt::WindowStaysOnTopHint);//窗口置顶

        QDialog::show();
    }

private:
    QLabel *m_label;
};

void CustomToast::show(CustomToast::Level level, const QString &text, int interval)
{
    m_toastDailog->show(level, text);
    if(m_timerId != 0)
    {
        //如果已开启一个定时器，先把他关掉
        killTimer(m_timerId);
    }
    m_timerId = startTimer(interval); //启动定时器，interval毫秒触发定时器事件，直到调用killTimer
}

CustomToast &CustomToast::instance()
{
    static CustomToast thisToast;//这种实例化方法会自动回收内存
    return thisToast;
}

CustomToast::CustomToast(QObject *parent) : QObject(parent)
{
    m_toastDailog = new ToastDailog;
}

void CustomToast::timerEvent(QTimerEvent *event)
{
    killTimer(m_timerId);
    m_timerId = 0;
    m_toastDailog->accept(); //隐藏模态对话框
}
