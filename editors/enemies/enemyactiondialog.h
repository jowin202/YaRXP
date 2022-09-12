#ifndef ENEMYACTIONDIALOG_H
#define ENEMYACTIONDIALOG_H

#include <QWidget>

#include "RXIO/RXObjects/rpgenemyaction.h"

namespace Ui {
class EnemyActionDialog;
}

class RPGSystem;
class EnemyActionDialog : public QWidget
{
    Q_OBJECT

public:
    explicit EnemyActionDialog(RPGSystem *system, RPGEnemyAction * action, QWidget *parent = nullptr);
    ~EnemyActionDialog();


signals:
    void ok_clicked();

private slots:
    void on_button_cancel_clicked();
    void on_button_ok_clicked();

private:
    Ui::EnemyActionDialog *ui;
    RPGSystem *system;
};

#endif // ENEMYACTIONDIALOG_H
