#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"

#include "enemyactiondialog.h"
#include "ui_enemyactiondialog.h"

EnemyActionDialog::EnemyActionDialog(RPGEditorController *ec, int row,
                                     int turn_a, int turn_b, int hp,
                                     int level, int switch_id, int kind,
                                     int rating, int skill, int basic, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnemyActionDialog)
{
    ui->setupUi(this);
    this->ec = ec;

    this->ui->switch_widget->setSwitchWidget(ec->get_db());
    this->ui->switch_widget->setValue(switch_id);

    this->row = row;

    if (turn_a != 0 || turn_b != 1)
    {
        this->ui->check_turn->setChecked(true);
        this->ui->spin_turn_a->setValue(turn_a);
        this->ui->spin_turn_b->setValue(turn_b);
    }

    if (hp < 100)
    {
        this->ui->check_hp->setChecked(true);
        this->ui->spin_hp->setValue(hp);
    }

    if (level > 1)
    {
        this->ui->check_level->setChecked(true);
        this->ui->spin_level->setValue(level);
    }

    if (switch_id > 0)
    {
        this->ui->check_switch->setChecked(true);
    }

    this->ui->radio_basic->setChecked(kind == 0);
    this->ui->combo_basic->setCurrentIndex(basic);

    this->ui->radio_skill->setChecked(kind == 1);
    this->ec->fill_combo(this->ui->combo_skill, RPGDB::SKILLS, true, 3, false);
    for (int i = 0; i < this->ui->combo_skill->count(); i++)
    {
        if (this->ui->combo_skill->itemData(i).toInt() == skill)
        {
            this->ui->combo_skill->setCurrentIndex(i);
            break;
        }
    }

    this->ui->spin_rating->setValue(rating);
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
    int turn_a = this->ui->spin_turn_a->value();
    int turn_b = this->ui->spin_turn_b->value();
    if (this->ui->check_turn->isChecked() == false)
    { turn_a = 0; turn_b = 1; }

    int hp = this->ui->spin_hp->value();
    if (this->ui->check_hp->isChecked() == false) hp = 100;

    int level = this->ui->spin_level->value();
    if (this->ui->check_level->isChecked() == false) level = 1;

    int switch_id = this->ui->switch_widget->getValue();
    if (this->ui->check_switch->isChecked() == false) switch_id = 0;



    emit ok_clicked(row,turn_a,turn_b,hp,level,switch_id,
                    this->ui->radio_basic->isChecked() == false ? 1 : 0,
                    this->ui->radio_skill->isChecked() == true ?  this->ui->combo_skill->currentData().toInt() : 1,
                    this->ui->spin_rating->value(),
                    this->ui->combo_basic->currentIndex());
    this->close();
}
