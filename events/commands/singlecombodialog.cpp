#include "singlecombodialog.h"
#include "ui_singlecombodialog.h"

#include "RXIO2/rpgdb.h"

SingleComboDialog::SingleComboDialog(RPGDB *db, int code, int current, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleComboDialog)
{
    ui->setupUi(this);
    this->code = code;
    if (code == 105)
    {
        this->setWindowTitle("Button Input Processing");
        this->ui->label->setText("Variable for Button Code: ");
        for (int i = 1; i < db->get_variable_names().count(); i++)
        {
            this->ui->comboBox->addItem(QString("%1: %2").arg(i,4,10,QChar('0')).arg(db->get_variable_name(i)), i);
            if (i == current)
                this->ui->comboBox->setCurrentIndex(this->ui->comboBox->count()-1);
        }
    }
    else if (code == 117)
    {
        this->setWindowTitle("Call Common Event");
        this->ui->label->setText("Common Event: ");
        db->fill_combo(this->ui->comboBox, RPGDB::COMMONEVENTS, true, 3);
        this->ui->comboBox->setCurrentIndex(current-1); //hope this works -.-
    }
    else if (code == 314)
    {
        this->setWindowTitle("Recover All");
        this->ui->label->setText("Actor: ");
        db->fill_combo(this->ui->comboBox, RPGDB::ACTORS, true, 3);
        this->ui->comboBox->insertItem(0, "Entire Party", 0);
        this->ui->comboBox->setCurrentIndex(current); //not minus 1 here, hope this works -.-
    }
    else if (code == 334)
    {
        this->setWindowTitle("Enemy Recover All");
        this->ui->label->setText("Enemy: ");
        this->ui->comboBox->addItems({ "Entire Troop", "1.", "2.", "3.", "4.", "5.", "6.", "7.", "8."});
        this->ui->comboBox->setCurrentIndex(current+1); //entire troop is coded as -1
    }
    else if (code == 335)
    {
        this->setWindowTitle("Enemy Appearance");
        this->ui->label->setText("Enemy: ");
        this->ui->comboBox->addItems({ "1.", "2.", "3.", "4.", "5.", "6.", "7.", "8."});
        this->ui->comboBox->setCurrentIndex(current);
    }
}

SingleComboDialog::~SingleComboDialog()
{
    delete ui;
}

void SingleComboDialog::on_button_cancel_clicked()
{
    this->close();
}


void SingleComboDialog::on_button_ok_clicked()
{
    QJsonArray array;
    if (code == 105 || code == 117) array.append(this->ui->comboBox->currentData().toInt());
    else if (code == 314 || code == 335) array.append(this->ui->comboBox->currentIndex());
    else //if (code == 334)
        array.append(this->ui->comboBox->currentIndex()-1); //subtract -1 because of decoding
    emit ok_clicked(array);
    this->close();
}

