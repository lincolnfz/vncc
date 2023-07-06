#ifndef PLAYDIALOG_H
#define PLAYDIALOG_H
#include <qdialog.h>
#include <qwindow.h>
#include <qevent.h>
#include <base/base.h>
#include "common/evideorender.h"

class PlayDialog : public QDialog, public virtual nbase::SupportWeakCallback
{
public:
    PlayDialog(QWidget *parent, Qt::WindowFlags f);
    virtual ~PlayDialog();

public Q_SLOTS:
    void handle_heightChange(int arg);

protected:
    void resizeEvent(QResizeEvent *) override;
    void testgl();

private:
    std::shared_ptr<eVideoRender> _video_render;
};

#endif // PLAYDIALOG_H
