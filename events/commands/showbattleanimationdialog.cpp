#include "showbattleanimationdialog.h"
#include "ui_showbattleanimationdialog.h"

#include "RXIO2/rpgdb.h"

ShowBattleAnimationDialog::ShowBattleAnimationDialog(RPGDB *db, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowBattleAnimationDialog)
{
    ui->setupUi(this);
    db->fill_combo(this->ui->combo_animation, RPGDB::ANIMATIONS, true, 3);

    if (parameters.at(0).toInt() == 0)
    {
        this->ui->radio_enemy->setChecked(true);
        this->ui->combo_enemy->setCurrentIndex(parameters.at(1).toInt()+1);
    }
    else if (parameters.at(0).toInt() == 1)
    {
        this->ui->radio_actor->setChecked(true);
        this->ui->combo_actor->setCurrentIndex(parameters.at(1).toInt()+1);
    }

    this->ui->combo_animation->setCurrentIndex(parameters.at(2).toInt()-1);
}

ShowBattleAnimationDialog::~ShowBattleAnimationDialog()
{
    delete ui;
}

void ShowBattleAnimationDialog::on_button_ok_clicked()
{
    QJsonArray p;
    if (this->ui->radio_enemy->isChecked())
    {
        p.append(0);
        p.append(this->ui->combo_enemy->currentIndex()-1);
    }
    else
    {
        p.append(1);
        p.append(this->ui->combo_actor->currentIndex()-1);
    }

    p.append(this->ui->combo_animation->currentIndex()+1);
    emit ok_clicked(p);
    this->close();
}


void ShowBattleAnimationDialog::on_button_cancel_clicked()
{
    this->close();
}

