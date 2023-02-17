#ifndef TWEENINGDIALOG_H
#define TWEENINGDIALOG_H

#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class TweeningDialog;
}

class TweeningDialog : public QWidget
{
    Q_OBJECT

public:
    explicit TweeningDialog(int max_frames, QWidget *parent = nullptr);
    ~TweeningDialog();
    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }

signals:
    void ok_clicked(int,int,int,int, bool, bool, bool);

private slots:
    void on_button_ok_clicked();

    void on_button_cancel_clicked();

private:
    Ui::TweeningDialog *ui;
};

#endif // TWEENINGDIALOG_H
