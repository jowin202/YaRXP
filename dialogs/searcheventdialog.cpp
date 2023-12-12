#include "searcheventdialog.h"
#include "ui_searcheventdialog.h"
#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgmapinfocontroller.h"
#include "RXIO2/searchthread.h"

SearchEventDialog::SearchEventDialog(RPGDB *db) :
    QWidget(),
    ui(new Ui::SearchEventDialog)
{
    ui->setupUi(this);
    this->ui->table_result->hideColumn(0);
    this->ui->table_result->hideColumn(1);
    this->ui->table_result->hideColumn(2);
    this->ui->table_result->hideColumn(3);

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
    int mode = -1;
    int sw_var = -1;
    if (this->ui->radio_text->isChecked()) mode = SearchThread::TEXT;
    else if (this->ui->radio_switch->isChecked())
    {
        mode = SearchThread::SWITCH;
        sw_var = this->ui->switch_widget->getValue();
    }
    else if (this->ui->radio_variable->isChecked())
    {
        mode = SearchThread::VARIABLE;
        sw_var = this->ui->variable_widget->getValue();
    }


    Qt::CaseSensitivity cs = Qt::CaseSensitive;
    if (this->ui->check_ignore_case->isChecked())
        cs = Qt::CaseInsensitive;


    SearchThread *thread = new SearchThread(db,mode,this->ui->line_text->text(),sw_var,cs);

    connect(thread, &SearchThread::finished, [=](){
        this->ui->table_result->clearContents();
        int num = (thread->result.count()) >> 2; //4 nums by res
        this->ui->table_result->setRowCount(num);

        for (int r = 0; r < num; r++)
        {
            int map_id = thread->result.at(4*r);
            int event_id = thread->result.at(4*r+1);
            int page_id = thread->result.at(4*r+2);
            int line_id = thread->result.at(4*r+3);
            QJsonObject event_obj = db->get_mapfile_by_id(map_id)->object().value("@events").toObject().value(QString::number(event_id)).toObject();
            QJsonObject command = event_obj.value("@pages").toArray().at(page_id).toObject().value("@list").toArray().at(line_id).toObject();
            QString map_cell = QString("%1 (%2)").arg(mic->get_name(map_id)).arg(map_id);
            QString event_cell = QString("Event %1 (%2), Page: %3").arg(event_obj.value("@name").toString()).arg(event_id).arg(page_id+1);
            QString content_cell = QString("%1").arg(command.value("@parameters").toArray().at(0).toString());

            this->ui->table_result->setItem(r,0, new QTableWidgetItem(QString::number(map_id)));
            this->ui->table_result->setItem(r,1, new QTableWidgetItem(QString::number(event_id)));
            this->ui->table_result->setItem(r,2, new QTableWidgetItem(QString::number(page_id)));
            this->ui->table_result->setItem(r,3, new QTableWidgetItem(QString::number(line_id)));
            this->ui->table_result->setItem(r,4, new QTableWidgetItem(map_cell));
            this->ui->table_result->setItem(r,5, new QTableWidgetItem(event_cell));
            this->ui->table_result->setItem(r,6, new QTableWidgetItem(content_cell));
        }

        this->ui->table_result->resizeColumnsToContents();
    });
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}


void SearchEventDialog::on_button_cancel_clicked()
{
    this->close();
}

