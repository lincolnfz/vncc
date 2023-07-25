#ifndef EMOUSEHOVER_H
#define EMOUSEHOVER_H

#include <QObject>

class emouseHover : public QObject
{
    Q_OBJECT
public:
    explicit emouseHover(QObject *parent = nullptr);

signals:
    void signal_sendBtnObj(QObject*, QString);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // EMOUSEHOVER_H
