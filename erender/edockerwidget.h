#ifndef EDOCKERWIDGET_H
#define EDOCKERWIDGET_H
#include <QDockWidget>

class eDockerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit eDockerWidget(QWidget *parent = nullptr);
    void SetTopWindow();

signals:
    void close_main_signal();
    void fps_signal(int);
    void bitrate_signal(int);
    void switch_dev_signal(QString);
    void home_signal();
    void process_signal();
    void return_signal();

public slots:
    void slot_MouseTrack(QObject*, QString);
    void quit_clicked(bool checked);
    void fps_30_click(bool checked);
    void fps_48_click(bool checked);
    void fps_60_click(bool checked);
    void q1_click(bool checked);
    void q2_click(bool checked);
    void q3_click(bool checked);
    void q4_click(bool checked);
    void home_click(bool checked);
    void Process_click(bool checked);
    void return_click(bool checked);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void timerEvent(QTimerEvent *event) override;

    void HideWindow();
    void ShowWindow();
    void resetTimer();


private:
    bool _bsize = false;
    int _init_w = 0;
    int _init_h = 0;
    int _timer_id = 0;

};

#endif // EDOCKERWIDGET_H
