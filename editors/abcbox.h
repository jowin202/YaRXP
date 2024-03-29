#ifndef ABCBOX_H
#define ABCBOX_H

#include <QWidget>

namespace Ui {
class ABCBox;
}

class ABCBox : public QWidget
{
    Q_OBJECT

public:
    explicit ABCBox(int value, QString text, QWidget *parent = nullptr);
    ~ABCBox();

    int getValue();


signals:
    void value_changed();

private slots:
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::ABCBox *ui;
};

#endif // ABCBOX_H
