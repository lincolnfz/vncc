#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <econtroltrans.h>
#include <base/base.h>
#include <QMainWindow>
#include <edockerwidget.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();
    //virtual QVariant inputMethodQuery(Qt::InputMethodQuery) const override;
    //virtual bool eventFilter(QObject *watched, QEvent *event) override;

protected:
    virtual void resizeEvent(QResizeEvent *) override;
    virtual void mousePressEvent(QMouseEvent *event) override; //鼠标按键按下
    virtual void mouseReleaseEvent(QMouseEvent *event) override; //鼠标按键抬起
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override; //鼠标按键双击
    virtual void mouseMoveEvent(QMouseEvent *event) override; //鼠标移动
    virtual void wheelEvent(QWheelEvent *event) override; //鼠标滚轮滚动
    virtual void inputMethodEvent(QInputMethodEvent *) override;
    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void closeEvent(QCloseEvent *event) override;

    void OutMouseMsg(float x, float y, int mtype);
    void OutTextMsg(QString& txt);
    void CtrlAccelrate(int keycode);
    void RButtonEvent(float x, float y, int mtype);

protected:
    std::shared_ptr<eControlTrans> _trans;
    eDockerWidget* _docker = nullptr;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
