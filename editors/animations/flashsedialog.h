#ifndef FLASHSEDIALOG_H
#define FLASHSEDIALOG_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonValue>
#include <QKeyEvent>

namespace Ui {
class FlashSEDialog;
}

class FlashSEDialog : public QWidget
{
    Q_OBJECT

public:
    explicit FlashSEDialog(QJsonObject timing, QWidget *parent = nullptr);
    ~FlashSEDialog();

    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->close();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }

private slots:
    void on_spin_red_valueChanged(int arg1);

    void on_spin_green_valueChanged(int arg1);

    void on_spin_blue_valueChanged(int arg1);

    void on_button_ok_clicked();

    void on_button_cancel_clicked();

private:
    Ui::FlashSEDialog *ui;
};

#endif // FLASHSEDIALOG_H
