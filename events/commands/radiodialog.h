#ifndef RADIODIALOG_H
#define RADIODIALOG_H

#include <QWidget>
#include <QKeyEvent>
#include <QJsonArray>

namespace Ui {
class RadioDialog;
}

class RadioDialog : public QWidget
{
    Q_OBJECT

public:
    explicit RadioDialog(QWidget *parent = nullptr, int code = 0, bool left = true);
    ~RadioDialog();

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
    Ui::RadioDialog *ui;
};

#endif // RADIODIALOG_H
