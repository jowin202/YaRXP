#ifndef ENEMYACTIONDIALOG_H
#define ENEMYACTIONDIALOG_H

#include <QWidget>
#include <QKeyEvent>

#include "RXIO/RXObjects/rpgenemyaction.h"

namespace Ui {
class EnemyActionDialog;
}

class RPGSystem;
class EnemyActionDialog : public QWidget
{
    Q_OBJECT

public:
    explicit EnemyActionDialog(RPGSystem *system, int row, int turn_a, int turn_b, int hp, int level, int switch_id, int kind, int skill, int rating, int basic, QWidget *parent = nullptr);
    ~EnemyActionDialog();


    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->close();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }


signals:
    void ok_clicked(int, int, int, int, int, int , int, int, int, int);

private slots:
    void on_button_cancel_clicked();
    void on_button_ok_clicked();

private:
    Ui::EnemyActionDialog *ui;
    RPGSystem *system;
    int row;
};

#endif // ENEMYACTIONDIALOG_H
