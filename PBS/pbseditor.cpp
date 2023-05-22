#include "pbseditor.h"
#include "ui_pbseditor.h"

#include "PBS/mapconnection/mapconnectiondialog.h"
#include "PBS/mapencounter/mapencounterdialog.h"

PBSEditor::PBSEditor(RPGDB *db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PBSEditor)
{
    ui->setupUi(this);
    this->db = db;
}

PBSEditor::~PBSEditor()
{
    delete ui;
}

void PBSEditor::on_button_mapConnection_clicked()
{
    MapConnectionDialog *d = new MapConnectionDialog(db);
    this->close();
    d->show();
}


void PBSEditor::on_button_mapencounter_clicked()
{
    MapEncounterDialog *d = new MapEncounterDialog(db);
    this->close();
    d->show();
}

