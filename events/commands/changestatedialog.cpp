#include "changestatedialog.h"
#include "ui_changestatedialog.h"

#include "RXIO2/rpgdb.h"

ChangeStateDialog::ChangeStateDialog(RPGDB *db, int code, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangeStateDialog)
{
    ui->setupUi(this);

    this->code = code;

    if (code == 318)
        db->fill_combo(this->ui->combo_state, RPGDB::SKILLS, true, 3);
    else
        db->fill_combo(this->ui->combo_state, RPGDB::STATES, true, 3);

    if (code == 313)
    {
        db->fill_combo(this->ui->comboBox, RPGDB::ACTORS, true, 3);
        this->ui->comboBox->insertItem(0, "Entire Party");
        this->ui->comboBox->setCurrentIndex(parameters.at(0).toInt());
    }
    else if (code == 333)
    {
        this->setWindowTitle("Change Enemy State");
        this->ui->comboBox->addItem("Entire Troop");
        for (int i = 0; i < 8; i++)
            this->ui->comboBox->addItem(QString("%1.").arg(i+1));
        this->ui->comboBox->setCurrentIndex(parameters.at(0).toInt()+1);
    }
    else if (code == 318)
    {
        this->setWindowTitle("Change Skills");
        this->ui->label_2->setText("Skill:");
        this->ui->radio_add->setText("Learn");
        this->ui->radio_remove->setText("Forget");
        db->fill_combo(this->ui->comboBox, RPGDB::ACTORS, true, 3);
        this->ui->comboBox->setCurrentIndex(parameters.at(0).toInt()-1);

    }

    if (parameters.at(1).toInt() == 0)
        this->ui->radio_add->setChecked(true);
    else
        this->ui->radio_remove->setChecked(true);

    this->ui->combo_state->setCurrentIndex(parameters.at(2).toInt()-1);
}

ChangeStateDialog::~ChangeStateDialog()
{
    delete ui;
}

void ChangeStateDialog::on_button_cancel_clicked()
{
    this->close();
}


void ChangeStateDialog::on_button_ok_clicked()
{
    QJsonArray p;
    if (code == 313)
        p.append(this->ui->comboBox->currentIndex());
    else if (code == 333)
        p.append(this->ui->comboBox->currentIndex()-1);
    else if (code == 318)
        p.append(this->ui->comboBox->currentIndex()+1);

    if (this->ui->radio_add->isChecked())
        p.append(0);
    else
        p.append(1);

    p.append(this->ui->combo_state->currentIndex()+1);
    this->close();
    emit ok_clicked(p);
}

