#ifndef CHANGEACTORNAMEDIALOG_H
#define CHANGEACTORNAMEDIALOG_H

#include <QWidget>
#include <QKeyEvent>
#include <QJsonArray>

class RPGDB;

namespace Ui {
class ChangeActorNameDialog;
}

class ChangeActorNameDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ChangeActorNameDialog(RPGDB *db, QJsonArray parameters, QWidget *parent = nullptr);
    ~ChangeActorNameDialog();
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
    Ui::ChangeActorNameDialog *ui;
};

#endif // CHANGEACTORNAMEDIALOG_H
