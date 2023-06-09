#include "trainertype.h"
#include "ui_trainertype.h"
#include "dialogs/audiodialog.h"
#include "PBS/pbsfactory.h"

TrainerType::TrainerType(QJsonArray type, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrainerType)
{
    ui->setupUi(this);

    QJsonObject obj = type.at(1).toObject();

    this->ui->spin_base_money->setValue(obj.value("@base_money").toInt());
    this->ui->spin_skill_level->setValue(obj.value("@skill_level").toInt());
    this->ui->line_battle_bgm->setText(obj.value("@battle_BGM").toString());
    this->ui->line_intro_bgm->setText(obj.value("@intro_BGM").toString());
    this->ui->line_victory_bgm->setText(obj.value("@victory_BGM").toString());

    this->ui->line_type_name->setText(obj.value("@real_name").toString());
    this->ui->combo_type_gender->setCurrentIndex(obj.value("@gender").toInt());


    this->ui->line_id->setText(obj.value("@id").toObject().value("symbol_value").toString());
}

TrainerType::~TrainerType()
{
    delete ui;
}

void TrainerType::on_button_intro_bgm_clicked()
{
    //AudioDialog *dialog = new AudioDialog(ec->get_db(), se.value("@name").toString(), se.value("@volume").toInt(), se.value("@pitch").toInt(), AudioDialog::SE, 0);
    //connect(dialog,SIGNAL(ok_clicked(QString,int,int)), this, SLOT(set_menu_se(QString,int,int)));
    //dialog->show();

}


void TrainerType::on_button_battle_bgm_clicked()
{

}


void TrainerType::on_button_victory_bgm_clicked()
{

}


void TrainerType::on_button_cancel_clicked()
{
    this->close();
}


void TrainerType::on_button_ok_clicked()
{
    QJsonArray result = PBSFactory().create_trainer_type(this->ui->line_id->text(), this->ui->line_type_name->text(),
                                                 this->ui->spin_base_money->value(), this->ui->spin_skill_level->value(),
                                                 this->ui->combo_type_gender->currentIndex(), this->ui->line_intro_bgm->text(),
                                                 this->ui->line_battle_bgm->text(), this->ui->line_victory_bgm->text());
    emit finished(result);
    this->close();
}

