#ifndef SPINSPINDIALOG_H
#define SPINSPINDIALOG_H

#include <QWidget>
#include <QKeyEvent>
#include <QJsonArray>

namespace Ui {
class SpinSpinDialog;
}

class SpinSpinDialog : public QWidget
{
    Q_OBJECT

public:
    explicit SpinSpinDialog(int code, QJsonArray parameters, QWidget *parent = nullptr);
    ~SpinSpinDialog();

    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }

signals:
    void ok_clicked(QJsonArray);

private slots:
    void on_button_cancel_clicked();

    void on_button_ok_clicked();

private:
    Ui::SpinSpinDialog *ui;
};

#endif // SPINSPINDIALOG_H
