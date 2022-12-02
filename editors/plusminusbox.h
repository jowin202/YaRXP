#ifndef PLUSMINUSBOX_H
#define PLUSMINUSBOX_H

#include <QDebug>
#include <QWidget>

#include <QToolButton>

namespace Ui {
class PlusMinusBox;
}

class PlusMinusBox : public QWidget
{
    Q_OBJECT

public:
    explicit PlusMinusBox(QString text, QWidget *parent = nullptr);
    ~PlusMinusBox();

    void setPlus();
    void setMinus();
    void setFalse();

    int value();

signals:
    void value_changed();

private slots:
    void on_toolButton_clicked();

private:
    Ui::PlusMinusBox *ui;
};

#endif // PLUSMINUSBOX_H
