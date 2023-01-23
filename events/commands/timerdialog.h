#ifndef TIMERDIALOG_H
#define TIMERDIALOG_H

#include <QWidget>
#include <QJsonArray>
#include <QKeyEvent>
namespace Ui {
class TimerDialog;
}

class TimerDialog : public QWidget
{
    Q_OBJECT

public:
    explicit TimerDialog(QJsonArray parameters, QWidget *parent = nullptr);
    ~TimerDialog();
    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }

signals:
    void ok_clicked(QJsonArray);

private slots:
    void on_button_ok_clicked();

    void on_button_cancel_clicked();

private:
    Ui::TimerDialog *ui;
};

#endif // TIMERDIALOG_H
