#ifndef CHANGEPARTYMEMBERDIALOG_H
#define CHANGEPARTYMEMBERDIALOG_H

#include <QWidget>
#include <QJsonArray>
#include <QKeyEvent>

class RPGDB;

namespace Ui {
class ChangePartyMemberDialog;
}

class ChangePartyMemberDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ChangePartyMemberDialog(RPGDB *db, QJsonArray parameters, QWidget *parent = nullptr);
    ~ChangePartyMemberDialog();

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
    Ui::ChangePartyMemberDialog *ui;
};

#endif // CHANGEPARTYMEMBERDIALOG_H
