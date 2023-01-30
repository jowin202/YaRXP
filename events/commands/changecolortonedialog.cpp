#include "changecolortonedialog.h"
#include "ui_changecolortonedialog.h"


#include "RXIO2/factory.h"

ChangeColorToneDialog::ChangeColorToneDialog(RPGDB *db, int code, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangeColorToneDialog)
{
    ui->setupUi(this);

    Q_UNUSED(db);
    this->code = code;

    if (code == 224)
    {
        //for screen flash everything must be >= 0
        this->ui->slider_red->setMinimum(0);
        this->ui->slider_green->setMinimum(0);
        this->ui->slider_blue->setMinimum(0);
        this->ui->spin_red->setMinimum(0);
        this->ui->spin_green->setMinimum(0);
        this->ui->spin_blue->setMinimum(0);
    }

    if (code != 234) //if not picture
    {
        this->ui->label_img->setVisible(false); //img number invisible
        this->ui->spin_img_num->setVisible(false);

        if (code == 205)
            this->setWindowTitle("Change Fog Color Tone");
        else if (code == 223)
            this->setWindowTitle("Change Screen Color Tone");
        else if (code == 224)
            this->setWindowTitle("Screen Flash");

        this->ui->spin_red->setValue(parameters.at(0).toObject().value("r").toInt());
        this->ui->spin_green->setValue(parameters.at(0).toObject().value("g").toInt());
        this->ui->spin_blue->setValue(parameters.at(0).toObject().value("b").toInt());
        this->ui->spin_gray->setValue(parameters.at(0).toObject().value("alpha_gray").toInt());

        this->ui->spin_time->setValue(parameters.at(1).toInt());
    }
    else //if it is picture
    {
        this->ui->spin_img_num->setValue(parameters.at(0).toInt());
        this->setWindowTitle("Change Picture Color Tone");


        this->ui->spin_red->setValue(parameters.at(1).toObject().value("r").toInt());
        this->ui->spin_green->setValue(parameters.at(1).toObject().value("g").toInt());
        this->ui->spin_blue->setValue(parameters.at(1).toObject().value("b").toInt());
        this->ui->spin_gray->setValue(parameters.at(1).toObject().value("alpha_gray").toInt());

        this->ui->spin_time->setValue(parameters.at(2).toInt());

    }


}

ChangeColorToneDialog::~ChangeColorToneDialog()
{
    delete ui;
}

void ChangeColorToneDialog::on_button_cancel_clicked()
{
    this->close();
}


void ChangeColorToneDialog::on_button_ok_clicked()
{
    QJsonArray p;

    if (code == 234)
        p.append(this->ui->spin_img_num->value());

    p.append(Factory().create_color_tone(this->ui->spin_red->value(), this->ui->spin_green->value(), this->ui->spin_blue->value(), this->ui->spin_gray->value(),code!=224));
    p.append(this->ui->spin_time->value());

    this->close();
    emit ok_clicked(p);
}

