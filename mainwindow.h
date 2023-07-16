#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <base/callback/callback.h>
#include "base/thread/threadmanager.h"
#include <QSettings>
#include "eWebScoketCli.h"

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

    void test_widget();

    void test_callback(const std::string& c, int idx);

    void RunPlayer();

private slots:
    void on_btn_conn_clicked();

    void on_btn_login_clicked();

    void on_lineEdit_returnPressed();

    void on_pwd_edit_returnPressed();

    void on_btn_login_pressed();

    void action_Login();

    void on_btn_back_clicked();

    void on_btn_launch_clicked();

private:
    Ui::MainWindow *ui;


    std::shared_ptr<MiscThrad> _misc_thd;
    QSettings* _settings = nullptr;
    //std::shared_ptr<eWebScoketCli> _sp_websocket;
    eWebScoketCli _websocket;
    int _avport = 0;
    int _adbport = 0;
    int _webrtcport = 0;
    std::string _ip;
    std::string _localp_ip;
};
#endif // MAINWINDOW_H
