#include "RXIO/RXObjects/rpgsystem.h"

#include "enemyactiondialog.h"
#include "ui_enemyactiondialog.h"

EnemyActionDialog::EnemyActionDialog(RPGSystem *system, RPGEnemyAction * action, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnemyActionDialog)
{
    ui->setupUi(this);
    this->system = system;

    if (action->condition_turn_a != 0 || action->condition_turn_b != 1)
        this->ui->check_turn->setEnabled(true);
}

EnemyActionDialog::~EnemyActionDialog()
{
    delete ui;
}

void EnemyActionDialog::on_button_cancel_clicked()
{
    this->close();
}

void EnemyActionDialog::on_button_ok_clicked()
{
    emit ok_clicked();
    this->close();
}
