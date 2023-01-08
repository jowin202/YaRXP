#ifndef CONDITIONALBRANCHDIALOG_H
#define CONDITIONALBRANCHDIALOG_H

#include <QWidget>

namespace Ui {
class ConditionalBranchDialog;
}

class ConditionalBranchDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ConditionalBranchDialog(QWidget *parent = nullptr);
    ~ConditionalBranchDialog();

private:
    Ui::ConditionalBranchDialog *ui;
};

#endif // CONDITIONALBRANCHDIALOG_H
