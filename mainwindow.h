#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <rpc/server.h>
#include <base/callback/callback.h>
#include "base/thread/threadmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MiscThrad : public nbase::Thread {
public:
    MiscThrad(){};
    virtual ~MiscThrad(){};

    virtual void Init() override {

    };

    /**
    * 虚函数，主线程退出时，做一些清理工作
    * @return void	无返回值
    */
    virtual void Cleanup() override {

    };
};


class MainWindow : public QMainWindow , public virtual nbase::SupportWeakCallback
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

protected:
    void GenerateRpcLayout();

    void test_widget();

    void test_callback(const std::string& c, int idx);

private slots:
    void on_btn_conn_clicked();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<rpc::server> _srv;
    unsigned short _port = 26186;

    std::shared_ptr<MiscThrad> _misc_thd;
};
#endif // MAINWINDOW_H
