#include "searcheventdialog.h"
#include "ui_searcheventdialog.h"
#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgmapinfocontroller.h"

SearchEventDialog::SearchEventDialog(RPGDB *db) :
    QWidget(),
    ui(new Ui::SearchEventDialog)
{
    ui->setupUi(this);

    this->db = db;
    this->mic = new RPGMapInfoController(db);

    this->ui->switch_widget->setSwitchWidget(db);
    this->ui->variable_widget->setVariableWidget(db);

    this->ui->line_text->setFocus();
}

SearchEventDialog::~SearchEventDialog()
{
    delete ui;
}

void SearchEventDialog::on_button_ok_clicked()
{
    QJsonArray toplevel_maps = this->mic->get_toplevel_maps();
    QJsonArray non_toplevel_maps = this->mic->get_child_maps();
    qDebug() << toplevel_maps;


}


void SearchEventDialog::on_button_cancel_clicked()
{
    this->close();
}

