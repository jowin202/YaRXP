#ifndef CELLBATCHDIALOG_H
#define CELLBATCHDIALOG_H

#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class CellBatchDialog;
}

class CellBatchDialog : public QWidget
{
    Q_OBJECT

public:
    explicit CellBatchDialog(int max_frames, QWidget *parent = nullptr);
    ~CellBatchDialog();
    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }

private slots:
    void on_button_ok_clicked();

    void on_button_cancel_clicked();

private:
    Ui::CellBatchDialog *ui;
};

#endif // CELLBATCHDIALOG_H
