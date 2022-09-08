#include "RXIO/RXObjects/rpgsystem.h"
#include "skilllearning.h"
#include "ui_skilllearning.h"

SkillLearning::SkillLearning(RPGSystem *system, int row, int level, int skill, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SkillLearning)
{
    ui->setupUi(this);
    this->system = system;

    this->system->datasource.fill_combo(this->ui->combo_skill, RPGSystem::SKILLS, true, 3, skill);
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
