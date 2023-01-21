#ifndef SINGLECOMBODIALOG_H
#define SINGLECOMBODIALOG_H

#include <QWidget>
#include <QKeyEvent>

#include <QJsonArray>

class RPGDB;

namespace Ui {
class SingleComboDialog;
}

class SingleComboDialog : public QWidget
{
    Q_OBJECT

public:
    explicit SingleComboDialog(RPGDB* db, int code, int current, QWidget *parent = nullptr);
    ~SingleComboDialog();

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
    Ui::SingleComboDialog *ui;
    int code;
};

#endif // SINGLECOMBODIALOG_H
