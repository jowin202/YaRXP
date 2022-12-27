#include "flashsedialog.h"
#include "ui_flashsedialog.h"

#include "dialogs/audiodialog.h"
#include "RXIO2/rpgdb.h"
#include "RXIO2/factory.h"
#include "RXIO2/rpgeditorcontroller.h"


FlashSEDialog::FlashSEDialog(RPGEditorController *ec, int row, QJsonObject timing, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FlashSEDialog)
{
    ui->setupUi(this);
    this->ui->label_color->setAutoFillBackground(true);

    this->ec = ec;
    this->timing = timing;
    this->row = row;

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

    this->ui->spin_frame->setValue(1+timing.value("@frame").toInt());//counting at 1
    this->ui->combo_condition->setCurrentIndex(timing.value("@condition").toInt());

    this->ui->line_se->setText(timing.value("@se").toObject().value("@name").toString());
    this->audiofile_volume = timing.value("@se").toObject().value("@volume").toInt();
    this->audiofile_pitch = timing.value("@se").toObject().value("@pitch").toInt();
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
    int scope = 0;
    if (this->ui->radio_target->isChecked()) scope = 1;
    else if (this->ui->radio_screen->isChecked()) scope = 2;
    else if (this->ui->radio_hide->isChecked()) scope = 3;

    emit ok_clicked(Factory().create_animation_timing(
                        this->ui->spin_frame->value()-1, //start counting at 1
                        Factory().create_audiofile(this->ui->line_se->text(), this->audiofile_volume, this->audiofile_pitch),
                        this->ui->combo_condition->currentIndex(),
                        scope,
                        this->ui->spin_red->value(),
                        this->ui->spin_green->value(),
                        this->ui->spin_blue->value(),
                        this->ui->spin_strength->value(),
                        this->ui->spin_duration->value()),
                    this->row);
    this->close();
}


void FlashSEDialog::on_button_cancel_clicked()
{
    this->close();
}


void FlashSEDialog::on_button_se_clicked()
{
    AudioDialog *audiodialog = new AudioDialog(ec->get_db(),timing.value("@se").toObject().value("@name").toString(),timing.value("@se").toObject().value("@volume").toInt(),timing.value("@se").toObject().value("@pitch").toInt(), AudioDialog::SE );
    audiodialog->show();
    connect(audiodialog, SIGNAL(ok_clicked(QString,int,int)), this, SLOT(set_audiofile(QString,int,int)));
}

void FlashSEDialog::set_audiofile(QString name, int volume, int pitch)
{
    this->ui->line_se->setText(name);
    this->audiofile_volume = volume;
    this->audiofile_pitch = pitch;
}

