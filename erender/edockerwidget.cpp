#include "edockerwidget.h"
#include <QPushButton>
#include <QHBoxLayout>

eDockerWidget::eDockerWidget(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout;

    QPushButton *button1 = new QPushButton("button1");
    QPushButton *button2 = new QPushButton("button2");

    layout->addWidget(button1);
    layout->addWidget(button2);
    setLayout(layout);
}
