#ifndef CELLPROPERTIESDIALOG_H
#define CELLPROPERTIESDIALOG_H

#include <QWidget>

namespace Ui {
class CellPropertiesDialog;
}

class CellPropertiesDialog : public QWidget
{
    Q_OBJECT

public:
    explicit CellPropertiesDialog(QWidget *parent = nullptr);
    ~CellPropertiesDialog();

private slots:
    void on_button_ok_clicked();

    void on_button_cancel_clicked();

private:
    Ui::CellPropertiesDialog *ui;
};

#endif // CELLPROPERTIESDIALOG_H
