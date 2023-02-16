#ifndef ENTIRESLIDEDIALOG_H
#define ENTIRESLIDEDIALOG_H

#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class EntireSlideDialog;
}

class EntireSlideDialog : public QWidget
{
    Q_OBJECT

public:
    explicit EntireSlideDialog(int max_frames, QWidget *parent = nullptr);
    ~EntireSlideDialog();
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
    Ui::EntireSlideDialog *ui;
};

#endif // ENTIRESLIDEDIALOG_H
