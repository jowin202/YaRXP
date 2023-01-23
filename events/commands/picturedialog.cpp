#include "picturedialog.h"
#include "ui_picturedialog.h"

#include "RXIO2/rpgdb.h"
#include "dialogs/imagedialog.h"

PictureDialog::PictureDialog(RPGDB *db, int code, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PictureDialog)
{
    ui->setupUi(this);

    this->code = code;
    this->db = db; //image dialog

    for (int i = 1; i < db->get_variable_names().count(); i++)
    {
        this->ui->combo_var_x->addItem(QString("%1: %2").arg(i,4,10,QChar('0')).arg(db->get_variable_name(i)));
        this->ui->combo_var_y->addItem(QString("%1: %2").arg(i,4,10,QChar('0')).arg(db->get_variable_name(i)));
    }


    if (code == 231)
    {
        this->setWindowTitle("Show Picture");
        this->ui->stackedWidget->setCurrentIndex(0);
        this->ui->spin_pic_num->setValue(parameters.at(0).toInt());
        this->ui->line_graphic->setText(parameters.at(1).toString());
    }
    else if (code == 232)
    {
        this->setWindowTitle("Move Picture");
        this->ui->spin_pic_num_2->setValue(parameters.at(0).toInt());
        this->ui->stackedWidget->setCurrentIndex(1);
        this->ui->spin_time->setValue(parameters.at(1).toInt());
    }

    if (parameters.at(2).toInt() == 0)
        this->ui->radio_upper_left->setChecked(true);
    else if (parameters.at(2).toInt() == 1)
        this->ui->radio_center->setChecked(true);

    if (parameters.at(3).toInt() == 0)
    {
        this->ui->radio_constant->setChecked(true);
        this->ui->combo_var_x->setEnabled(false);
        this->ui->combo_var_y->setEnabled(false);
        this->ui->spin_const_x->setValue(parameters.at(4).toInt());
        this->ui->spin_const_y->setValue(parameters.at(5).toInt());
    }
    else if (parameters.at(3).toInt() == 1)
    {
        this->ui->radio_variable->setChecked(true);
        this->ui->spin_const_x->setEnabled(false);
        this->ui->spin_const_y->setEnabled(false);
        this->ui->combo_var_x->setCurrentIndex(parameters.at(4).toInt()-1);
        this->ui->combo_var_y->setCurrentIndex(parameters.at(5).toInt()-1);
    }

    this->ui->spin_zoom_x->setValue(parameters.at(6).toInt());
    this->ui->spin_zoom_y->setValue(parameters.at(7).toInt());
    this->ui->spin_opacity->setValue(parameters.at(8).toInt());
    this->ui->combo_blending->setCurrentIndex(parameters.at(9).toInt());

}

PictureDialog::~PictureDialog()
{
    delete ui;
}

void PictureDialog::on_button_cancel_clicked()
{
    this->close();
}


void PictureDialog::on_button_ok_clicked()
{
    QJsonArray p;


    if (code == 231)
    {
        p.append(this->ui->spin_pic_num->value());
        p.append(this->ui->line_graphic->text());
    }
    else if (code == 232)
    {
        p.append(this->ui->spin_pic_num_2->value());
        p.append(this->ui->spin_time->value());
    }

    if (this->ui->radio_upper_left->isChecked())
        p.append(0);
    else //center
        p.append(1);

    if (this->ui->radio_constant->isChecked())
    {
        p.append(0);
        p.append(this->ui->spin_const_x->value());
        p.append(this->ui->spin_const_y->value());
    }
    else //variable
    {
        p.append(1);
        p.append(this->ui->combo_var_x->currentIndex()+1);
        p.append(this->ui->combo_var_y->currentIndex()+1);
    }

    p.append(this->ui->spin_zoom_x->value());
    p.append(this->ui->spin_zoom_y->value());
    p.append(this->ui->spin_opacity->value());
    p.append(this->ui->combo_blending->currentIndex());


    ok_clicked(p);
    this->close();
}


void PictureDialog::on_change_pic_clicked()
{
    ImageDialog *dialog = new ImageDialog(db, ImageDialog::PICTURES, this->ui->line_graphic->text());
    dialog->show();

    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_graphic, SLOT(setText(QString)));
}

