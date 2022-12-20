#include "flashsedialog.h"
#include "ui_flashsedialog.h"

FlashSEDialog::FlashSEDialog(QJsonObject timing, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FlashSEDialog)
{
    ui->setupUi(this);
    this->ui->label_color->setAutoFillBackground(true);

    QJsonObject flash_color = timing.value("@flash_color").toObject();
    this->ui->spin_red->setValue(flash_color.value("r").toInt());
    this->ui->spin_green->setValue(flash_color.value("g").toInt());
    this->ui->spin_blue->setValue(flash_color.value("b").toInt());
    this->ui->spin_strength->setValue(flash_color.value("alpha_gray").toInt());

    this->ui->spin_duration->setValue(timing.value("@flash_duration").toInt());

    if (timing.value("@flash_scope").toInt() == 0) this->ui->radio_none->setChecked(true);
    else if (timing.value("@flash_scope").toInt() == 1) this->ui->radio_target->setChecked(true);
    else if (timing.value("@flash_scope").toInt() == 2) this->ui->radio_screen->setChecked(true);
    else if (timing.value("@flash_scope").toInt() == 3) this->ui->radio_hide->setChecked(true);

}

FlashSEDialog::~FlashSEDialog()
{
    delete ui;
}

void FlashSEDialog::on_spin_red_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    QColor col(this->ui->spin_red->value(),this->ui->spin_green->value(),this->ui->spin_blue->value());
    QPalette pal;
    pal.setColor(ui->label_color->backgroundRole(), col);
    this->ui->label_color->setPalette(pal);
}


void FlashSEDialog::on_spin_green_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    QColor col(this->ui->spin_red->value(),this->ui->spin_green->value(),this->ui->spin_blue->value());
    QPalette pal;
    pal.setColor(ui->label_color->backgroundRole(), col);
    this->ui->label_color->setPalette(pal);
}


void FlashSEDialog::on_spin_blue_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    QColor col(this->ui->spin_red->value(),this->ui->spin_green->value(),this->ui->spin_blue->value());
    QPalette pal;
    pal.setColor(ui->label_color->backgroundRole(), col);
    this->ui->label_color->setPalette(pal);
}


void FlashSEDialog::on_button_ok_clicked()
{

}


void FlashSEDialog::on_button_cancel_clicked()
{
    this->close();
}

