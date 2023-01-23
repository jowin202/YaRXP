#ifndef CHOICESDIALOG_H
#define CHOICESDIALOG_H

#include <QWidget>

namespace Ui {
class ChoicesDialog;
}

class ChoicesDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ChoicesDialog(QWidget *parent = nullptr);
    ~ChoicesDialog();

private:
    Ui::ChoicesDialog *ui;
};

#endif // CHOICESDIALOG_H
