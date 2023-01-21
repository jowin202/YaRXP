#ifndef INCREASEDECREASEDIALOG_H
#define INCREASEDECREASEDIALOG_H

#include <QWidget>
#include <QKeyEvent>
#include <QJsonArray>


class RPGDB;

namespace Ui {
class IncreaseDecreaseDialog;
}

class IncreaseDecreaseDialog : public QWidget
{
    Q_OBJECT

public:
    explicit IncreaseDecreaseDialog(RPGDB *db, int code, QJsonArray parameters, QWidget *parent = nullptr);
    ~IncreaseDecreaseDialog();

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
    Ui::IncreaseDecreaseDialog *ui;
    int code;
};

#endif // INCREASEDECREASEDIALOG_H
