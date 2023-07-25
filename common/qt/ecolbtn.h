#ifndef ECOLBTN_H
#define ECOLBTN_H
#include <QPushButton>

class ecolbtn : public QPushButton
{
    Q_OBJECT
public:
    enum class STATE{
      DEF = 0,
      SEL = 1,
    };
    explicit ecolbtn(const int w, const int h, const QString& defcol, const QString& selcol, const QString &text, QWidget *parent=nullptr);
    void SetState(STATE s);

private:
    QString _def_style;
    QString _sel_style;

    STATE _s = STATE::DEF;
};

#endif // ECOLBTN_H
