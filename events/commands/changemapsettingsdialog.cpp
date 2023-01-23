#include "changemapsettingsdialog.h"
#include "ui_changemapsettingsdialog.h"

#include "dialogs/imagedialog.h"
#include "RXIO2/rpgdb.h"

ChangeMapSettingsDialog::ChangeMapSettingsDialog(RPGDB *db, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangeMapSettingsDialog)
{
    ui->setupUi(this);

    this->db = db;
    if (parameters.at(0).toInt() == 0)
    {
        this->ui->radio_panorama->setChecked(true);
        this->ui->line_panorama->setText(parameters.at(1).toString());
        this->hue = parameters.at(2).toInt();
    }
    else if (parameters.at(0).toInt() == 1)
    {
        this->ui->radio_fog->setChecked(true);
        this->ui->line_fog->setText(parameters.at(1).toString());
        this->hue = parameters.at(2).toInt();
        this->opacity = parameters.at(3).toInt();
        this->blending = parameters.at(4).toInt();
        this->zoom = parameters.at(5).toInt();
        this->sx = parameters.at(6).toInt();
        this->sy = parameters.at(7).toInt();
    }
    else //if (parameters.at(0).toInt() == 3)
    {
        this->ui->radio_battleback->setChecked(true);
        this->ui->line_battleback->setText(parameters.at(1).toString());
    }


}

ChangeMapSettingsDialog::~ChangeMapSettingsDialog()
{
    delete ui;
}


void ChangeMapSettingsDialog::on_button_cancel_clicked()
{
    this->close();
}


void ChangeMapSettingsDialog::on_button_ok_clicked()
{
    QJsonArray p;

    if (this->ui->radio_panorama->isChecked())
    {
        p.append(0);
        p.append(this->ui->line_panorama->text());
        p.append(this->hue);
    }
    else if (this->ui->radio_fog->isChecked())
    {
        p.append(1);
        p.append(this->ui->line_fog->text());
        p.append(this->hue);
        p.append(this->opacity);
        p.append(this->blending);
        p.append(this->zoom);
        p.append(this->sx);
        p.append(this->sy);
    }
    else if (this->ui->radio_battleback->isChecked())
    {
        p.append(2);
        p.append(this->ui->line_battleback->text());
    }
    emit ok_clicked(p);
    this->close();
}


void ChangeMapSettingsDialog::on_button_panorama_clicked()
{
    ImageDialog *dialog = new ImageDialog(db, ImageDialog::PANOMRAMAS, this->ui->line_panorama->text());
    dialog->set_hue(this->hue);
    connect(dialog, &ImageDialog::ok_clicked, [=](QString name) { this->ui->line_panorama->setText(name);});
    connect(dialog, &ImageDialog::ok_clicked_with_hue, [=](int hue) { this->hue = hue;});
    dialog->show();
}




void ChangeMapSettingsDialog::on_button_fog_clicked()
{
    ImageDialog *dialog = new ImageDialog(db, ImageDialog::FOGS, this->ui->line_fog->text());
    dialog->set_hue(this->hue);
    dialog->set_fog_options(this->sx, this->sy, this->opacity, this->zoom, this->blending);
    connect(dialog, &ImageDialog::ok_clicked, [=](QString name) { this->ui->line_fog->setText(name);});
    connect(dialog, &ImageDialog::ok_clicked_with_hue, [=](int hue) { this->hue = hue;});
    connect(dialog, &ImageDialog::ok_clicked_with_fog_options, [=](int sx, int sy, int opacity, int zoom, int blending) {
        this->sx = sx;
        this->sy = sy;
        this->opacity = opacity;
        this->zoom = zoom;
        this->blending = blending;
    });
    dialog->show();
}


void ChangeMapSettingsDialog::on_button_battleback_clicked()
{
    ImageDialog *dialog = new ImageDialog(db, ImageDialog::BATTLEBACKS, this->ui->line_battleback->text());
    connect(dialog, &ImageDialog::ok_clicked, [=](QString name) { this->ui->line_battleback->setText(name);});
    dialog->show();

}

