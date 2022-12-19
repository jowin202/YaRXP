#ifndef TWEENINGDIALOG_H
#define TWEENINGDIALOG_H

#include <QWidget>

namespace Ui {
class TweeningDialog;
}

class TweeningDialog : public QWidget
{
    Q_OBJECT

public:
    explicit TweeningDialog(QWidget *parent = nullptr);
    ~TweeningDialog();

private:
    Ui::TweeningDialog *ui;
};

#endif // TWEENINGDIALOG_H
