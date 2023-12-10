#include "searcheventdialog.h"
#include "ui_searcheventdialog.h"
#include "RXIO2/rpgdb.h"
#include "RXIO2/searchthread.h"

SearchEventDialog::SearchEventDialog(RPGDB *db) :
    QWidget(),
    ui(new Ui::SearchEventDialog)
{
    ui->setupUi(this);

    this->db = db;

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
    int mode = -1;
    if (this->ui->radio_text->isChecked()) mode = SearchThread::TEXT;

    Qt::CaseSensitivity cs = Qt::CaseSensitive;
    if (this->ui->check_ignore_case->isChecked())
        cs = Qt::CaseInsensitive;


    SearchThread *thread = new SearchThread(db,mode,this->ui->line_text->text(),0,cs); //TODO
    thread->run();
}


void SearchEventDialog::on_button_cancel_clicked()
{
    this->close();
}

