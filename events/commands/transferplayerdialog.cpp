#include "transferplayerdialog.h"
#include "ui_transferplayerdialog.h"

#include "dialogs/maplocationfinderdialog.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgmapinfocontroller.h"

TransferPlayerDialog::TransferPlayerDialog(RPGDB *db, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransferPlayerDialog)
{
    ui->setupUi(this);

    this->db = db;
    this->ui->widget_var_id->setVariableWidget(db);
    this->ui->widget_var_x->setVariableWidget(db);
    this->ui->widget_var_y->setVariableWidget(db);

    //qDebug() << parameters;

    if (parameters.at(0).toInt() == 0)
    {
        this->ui->radio_direct->setChecked(true);
        this->set_direct(parameters.at(1).toInt(),parameters.at(2).toInt(),parameters.at(3).toInt());
    }
    else
    {
        this->ui->radio_variable->setChecked(true);
        this->ui->widget_var_id->setValue(parameters.at(1).toInt());
        this->ui->widget_var_x->setValue(parameters.at(2).toInt());
        this->ui->widget_var_y->setValue(parameters.at(3).toInt());
    }

    this->ui->combo_direction->setCurrentIndex(parameters.at(4).toInt()/2);
    this->ui->combo_fade->setCurrentIndex(parameters.at(5).toInt());
}

TransferPlayerDialog::~TransferPlayerDialog()
{
    delete ui;
}

void TransferPlayerDialog::set_direct(int id, int x, int y)
{
    RPGMapInfoController mic(db);
    this->x = x;
    this->y = y;
    this->id = id;
    this->ui->line_direct->setText(QString("%1: %2 (%3,%4)").arg(id,3,10,QChar('0')).arg(mic.get_name(id)).arg(x,3,10,QChar('0')).arg(y,3,10,QChar('0')));
}

void TransferPlayerDialog::on_button_ok_clicked()
{
    QJsonArray p;
    if (this->ui->radio_direct->isChecked())
    {
        p.append(0);
        p.append(id);
        p.append(x);
        p.append(y);
    }
    else
    {
        p.append(1);
        p.append(this->ui->widget_var_id->getValue());
        p.append(this->ui->widget_var_x->getValue());
        p.append(this->ui->widget_var_y->getValue());
    }

    p.append(this->ui->combo_direction->currentIndex()*2);
    p.append(this->ui->combo_fade->currentIndex());

    this->close();
    emit ok_clicked(p);
}


void TransferPlayerDialog::on_button_cancel_clicked()
{
    this->close();
}


void TransferPlayerDialog::on_button_change_location_clicked()
{
    MapLocationFinderDialog *dialog = new MapLocationFinderDialog(db,id,x,y, true);
    dialog->show();
    connect(dialog, &MapLocationFinderDialog::ok_clicked, [=](int id, int x, int y){
       this->set_direct(id,x,y);
    });
}

