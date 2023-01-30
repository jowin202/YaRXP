#ifndef INPUTNUMBERDIALOG_H
#define INPUTNUMBERDIALOG_H

#include <QWidget>
#include <QJsonArray>
#include <QKeyEvent>

class RPGDB;

namespace Ui {
class InputNumberDialog;
}

class InputNumberDialog : public QWidget
{
    Q_OBJECT

public:
    explicit InputNumberDialog(RPGDB *db, QJsonArray parameters, QWidget *parent = nullptr);
    ~InputNumberDialog();

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
    Ui::InputNumberDialog *ui;
};

#endif // INPUTNUMBERDIALOG_H
