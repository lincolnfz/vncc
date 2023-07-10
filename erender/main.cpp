#include "mainwindow.h"

#include <QApplication>
#include <qevent.h>
#include <qlogging.h>
#include <qobject.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;


    //w.setWindowFlags(Qt::FramelessWindowHint);
    w.show();
    return a.exec();
}
