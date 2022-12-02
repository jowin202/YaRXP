#include "skilllearning.h"
#include "ui_skilllearning.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"

SkillLearning::SkillLearning(RPGEditorController *ec, int row, int level, int skill, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SkillLearning)
{
    ui->setupUi(this);
    this->ec = ec;

    this->ec->fill_combo(this->ui->combo_skill, RPGDB::SKILLS, true, 3, false);
    for (int i = 0; i < this->ui->combo_skill->count(); i++)
    {
        if (this->ui->combo_skill->itemData(i).toInt() == skill)
        {
            this->ui->combo_skill->setCurrentIndex(i);
            break;
        }
    }

    this->ui->spin_level->setValue(level);
    this->row = row;
}

SkillLearning::~SkillLearning()
{
    delete ui;
}

void SkillLearning::on_button_ok_clicked()
{
    emit ok_clicked(this->row,this->ui->spin_level->value(), this->ui->combo_skill->currentData().toInt());
    this->close();
}

void SkillLearning::on_button_cancel_clicked()
{
    this->close();
}
