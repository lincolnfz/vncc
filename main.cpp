#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    char *p=getenv("LD_LIBRARY_PATH");
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return a.exec();
}
