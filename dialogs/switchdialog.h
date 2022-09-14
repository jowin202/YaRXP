#ifndef SWITCHDIALOG_H
#define SWITCHDIALOG_H

#include <QWidget>

namespace Ui {
class SwitchDialog;
}

class SwitchDialog : public QWidget
{
    Q_OBJECT

public:
    explicit SwitchDialog(QWidget *parent = nullptr);
    ~SwitchDialog();

private:
    Ui::SwitchDialog *ui;
};

#endif // SWITCHDIALOG_H
