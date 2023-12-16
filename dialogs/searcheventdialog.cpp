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


    connect(thread, &SearchThread::finished, this, &SearchEventDialog::update_gui);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}


void SearchEventDialog::on_button_cancel_clicked()
{
    this->close();
}

void SearchEventDialog::update_gui(QList<int> result)
{
    //qDebug() << (QThread::currentThread() == QCoreApplication::instance()->thread());
    this->ui->table_result->clearContents();
    int num = (result.count()) >> 2; //4 nums by res
    this->ui->table_result->setRowCount(num);

    for (int r = 0; r < num; r++)
    {
        int map_id = result.at(4*r);
        int event_id = result.at(4*r+1);
        int page_id = result.at(4*r+2);
        int line_id = result.at(4*r+3);
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
        this->ui->table_result->item(r,4)->setFlags(this->ui->table_result->item(r,4)->flags() & ~Qt::ItemIsEditable);
        this->ui->table_result->item(r,5)->setFlags(this->ui->table_result->item(r,5)->flags() & ~Qt::ItemIsEditable);
        this->ui->table_result->item(r,6)->setFlags(this->ui->table_result->item(r,6)->flags() & ~Qt::ItemIsEditable);
    }
    this->ui->table_result->resizeColumnsToContents();
}


void SearchEventDialog::on_table_result_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    QTableWidgetItem *item1 = this->ui->table_result->item(row,0);
    QTableWidgetItem *item2 = this->ui->table_result->item(row,1);
    QTableWidgetItem *item3 = this->ui->table_result->item(row,2);
    QTableWidgetItem *item4 = this->ui->table_result->item(row,3);
    if (item1 != 0 && item2 != 0 && item3 != 0 && item4 != 0)
        emit select_event(item1->text().toInt(),item2->text().toInt(),item3->text().toInt(),item4->text().toInt());
}

