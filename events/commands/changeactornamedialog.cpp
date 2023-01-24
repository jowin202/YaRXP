#include "changeactornamedialog.h"
#include "ui_changeactornamedialog.h"

#include "RXIO2/rpgdb.h"

ChangeActorNameDialog::ChangeActorNameDialog(RPGDB *db, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangeActorNameDialog)
{
    ui->setupUi(this);
    db->fill_combo(this->ui->combo_actor, RPGDB::ACTORS, true, 3);
    this->ui->combo_actor->setCurrentIndex(parameters.at(0).toInt()-1);
    this->ui->line_name->setText(parameters.at(1).toString());
}

ChangeActorNameDialog::~ChangeActorNameDialog()
{
    delete ui;
}

void ChangeActorNameDialog::on_button_cancel_clicked()
{
    this->close();
}


void ChangeActorNameDialog::on_button_ok_clicked()
{
    QJsonArray p;
    p.append(this->ui->combo_actor->currentIndex()+1);
    p.append(this->ui->line_name->text());
    emit ok_clicked(p);
    this->close();
}

