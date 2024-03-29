#ifndef SHOWTEXTDIALOG_H
#define SHOWTEXTDIALOG_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QKeyEvent>

namespace Ui {
class ShowTextDialog;
}

class ShowTextDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ShowTextDialog(QWidget *parent = nullptr, int code = 101);
    ~ShowTextDialog();

    void setString(QString text);

    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }

signals:
    void ok_clicked(QString);

private slots:
    void on_button_ok_clicked();
    void on_button_cancel_clicked();

private:
    Ui::ShowTextDialog *ui;
};

#endif // SHOWTEXTDIALOG_H
