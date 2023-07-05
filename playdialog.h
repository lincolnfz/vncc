#ifndef PLAYDIALOG_H
#define PLAYDIALOG_H
#include <qdialog.h>

class PlayDialog : public QDialog
{
public:
    PlayDialog(QWidget *parent, Qt::WindowFlags f);
    virtual ~PlayDialog();

public Q_SLOTS:
    virtual void open() override;
};

#endif // PLAYDIALOG_H
