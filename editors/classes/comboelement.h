#ifndef COMBOELEMENT_H
#define COMBOELEMENT_H

#include <QWidget>
#include <QDebug>

namespace Ui {
class ComboElement;
}

class ComboElement : public QWidget
{
    Q_OBJECT

public:
    explicit ComboElement(int value, QString text,QWidget *parent = nullptr);
    ~ComboElement();


    int getValue();

private:
    Ui::ComboElement *ui;
};

#endif // COMBOELEMENT_H
