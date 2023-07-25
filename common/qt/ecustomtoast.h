#ifndef ECUSTOMTOAST_H
#define ECUSTOMTOAST_H

#endif // ECUSTOMTOAST_H
#ifndef CUSTOMTOAST_H
#define CUSTOMTOAST_H

#include <QObject>
#include <QRect>

class ToastDailog;

class CustomToast : public QObject
{
    Q_OBJECT
public:
    enum Level {INFO, WARN, ERROR};
    void show(Level level, const QString& text, int interval = 2000);
    static CustomToast &instance();

private:
    explicit CustomToast(QObject *parent = nullptr);
    void timerEvent(QTimerEvent *event) override;

private:
    ToastDailog *m_toastDailog;
    int m_timerId{0};
    QRect m_gemoetry;

};

#endif // CUSTOMTOAST_H
