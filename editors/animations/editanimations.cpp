#include "editanimations.h"
#include "ui_editanimations.h"

#include "dialogs/imagedialog.h"
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

    this->ui->line_name->setText(current_animation->name);
    this->ui->line_animation->setText(current_animation->animation_name);
    //dont fortget animation hue
    this->ui->combo_pos->setCurrentIndex(current_animation->position);
    this->ui->spin_frames->setValue(current_animation->frame_max);
}

void EditAnimations::on_button_animation_clicked()
{
    /*
    ImageDialog *imdialog = new ImageDialog(system, ImageDialog::ANIMATIONS,
                                            this->ui->line_animation->text());
    connect(imdialog, SIGNAL(ok_clicked(QString)), this->ui->line_animation, SLOT(setText(QString)));
    imdialog->show();
    */
}
