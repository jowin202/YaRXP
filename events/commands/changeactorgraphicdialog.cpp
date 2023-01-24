#include "changeactorgraphicdialog.h"
#include "ui_changeactorgraphicdialog.h"

#include "dialogs/imagedialog.h"
#include "RXIO2/rpgdb.h"

ChangeActorGraphicDialog::ChangeActorGraphicDialog(RPGDB *db, QJsonArray parameter, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangeActorGraphicDialog)
{
    ui->setupUi(this);
    this->db = db;
    db->fill_combo(this->ui->combo_actor, RPGDB::ACTORS, true, 3);
    this->ui->combo_actor->setCurrentIndex(parameter.at(0).toInt()-1);
    this->ui->line_character->setText(parameter.at(1).toString());
    this->ui->line_battler->setText(parameter.at(3).toString());

    this->hue1 = parameter.at(2).toInt();
    this->hue2 = parameter.at(4).toInt();
}

ChangeActorGraphicDialog::~ChangeActorGraphicDialog()
{
    delete ui;
}




void ChangeActorGraphicDialog::on_button_cancel_clicked()
{
    this->close();
}


void ChangeActorGraphicDialog::on_button_ok_clicked()
{
    QJsonArray p;
    p.append(this->ui->combo_actor->currentIndex()+1);
    p.append(this->ui->line_character->text());
    p.append(this->hue1);
    p.append(this->ui->line_battler->text());
    p.append(this->hue2);
    emit ok_clicked(p);
    this->close();
}


void ChangeActorGraphicDialog::on_button_character_clicked()
{
    ImageDialog *dialog = new ImageDialog(db, ImageDialog::CHARACTERS, this->ui->line_character->text());
    dialog->set_hue(hue1);
    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_character, SLOT(setText(QString)));
    connect(dialog, &ImageDialog::ok_clicked_with_hue, [=](int hue) {this->hue1 = hue; });
}



void ChangeActorGraphicDialog::on_button_battler_clicked()
{
    ImageDialog *dialog = new ImageDialog(db, ImageDialog::BATTLERS, this->ui->line_battler->text());
    dialog->set_hue(hue2);
    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_battler, SLOT(setText(QString)));
    connect(dialog, &ImageDialog::ok_clicked_with_hue, [=](int hue) {this->hue2 = hue; });
}

