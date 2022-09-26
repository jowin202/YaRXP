#include "editanimations.h"
#include "ui_editanimations.h"

#include "RXIO/RXObjects/rpgsystem.h"

EditAnimations::EditAnimations(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditAnimations)
{
    ui->setupUi(this);
}

EditAnimations::~EditAnimations()
{
    delete ui;
}

void EditAnimations::set_animation(int n)
{
    if (this->system->animation_list.length() <= n) return;
    RPGAnimation *current_animation = this->system->animation_list.at(n);
}
