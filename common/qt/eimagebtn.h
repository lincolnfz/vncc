#ifndef EIMAGEBTN_H
#define EIMAGEBTN_H
#include <QPushButton>
#include <QLabel>

class eImageBtn : public QPushButton
{
    Q_OBJECT
public:
    eImageBtn(const int w, const int h,
              const QString& imgurl, const QString& sel_imgurl,
              const QString &text, const QString &deftxtcol,
              const QString &seltextcol, QWidget *parent = nullptr);
    void Sel(bool sel);

private:
    QString _img_def, _img_sel;
    QString _text_def, _text_sel;

    QLabel *buttonImage, *buttonTxt;
};

#endif // EIMAGEBTN_H
