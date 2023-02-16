#ifndef COPYFRAMESDIALOG_H
#define COPYFRAMESDIALOG_H

#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class CopyFramesDialog;
}

class CopyFramesDialog : public QWidget
{
    Q_OBJECT

public:
    explicit CopyFramesDialog(int max_frames, bool true_for_copy_false_for_clear, QWidget *parent = nullptr);
    ~CopyFramesDialog();
    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }



signals:
    void ok_clicked(int,int,int);

private slots:
    void on_button_ok_clicked();
    void on_button_cancel_clicked();
    void update();

private:
    Ui::CopyFramesDialog *ui;
};

#endif // COPYFRAMESDIALOG_H
