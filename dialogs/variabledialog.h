#ifndef VARIABLEDIALOG_H
#define VARIABLEDIALOG_H

#include <QWidget>

namespace Ui {
class VariableDialog;
}

class VariableDialog : public QWidget
{
    Q_OBJECT

public:
    explicit VariableDialog(QWidget *parent = nullptr);
    ~VariableDialog();

private:
    Ui::VariableDialog *ui;
};

#endif // VARIABLEDIALOG_H
