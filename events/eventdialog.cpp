#include "eventdialog.h"
#include "ui_eventdialog.h"

#include "eventpage.h"
#include "RXIO2/rpgmapcontroller.h"


EventDialog::EventDialog(RPGMapController *mc, QJsonObject event, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventDialog)
{
    ui->setupUi(this);

    this->event = event;
    this->mc = mc;

    /*
    //for Debugging
    QJsonObject event_copy = QJsonObject(event);
    event_copy.remove("@name");
    event_copy.remove("@x");
    event_copy.remove("@y");
    event_copy.remove("@id");
    QCryptographicHash hash(QCryptographicHash::Sha3_256);
    hash.addData(QJsonDocument(event_copy).toJson(QJsonDocument::Compact));
    qDebug() << hash.result().toHex();
    */


    this->setWindowTitle(QString("YaRXP - Event %1").arg(event.value("@id").toInt(),3,10,QChar('0')));
    QJsonArray pages = event.value("@pages").toArray();

    for (int i = 0; i < pages.count(); i++)
    {
        this->ui->tab_widget->addTab(new EventPage(pages.at(i).toObject(), mc), QString());
    }

    this->ui->line_name->setText(event.value("@name").toString());

    this->do_numbers_of_tabs_right();
}

EventDialog::~EventDialog()
{
    delete ui;
}

void EventDialog::do_numbers_of_tabs_right()
{
    for (int i = 0; i < this->ui->tab_widget->count(); i++)
    {
        this->ui->tab_widget->setTabText(i,QString::number(i+1));
    }

    if (this->ui->tab_widget->count() <= 1) this->ui->button_delete->setEnabled(false);
    else this->ui->button_delete->setEnabled(true);

}

void EventDialog::on_button_new_clicked()
{
    int pos = this->ui->tab_widget->addTab(new EventPage(mc), QString()); //empty
    this->do_numbers_of_tabs_right();
    this->ui->tab_widget->setCurrentIndex(pos);
}

void EventDialog::on_button_delete_clicked()
{
    if (this->ui->tab_widget->count() > 1)
    {
        this->ui->tab_widget->removeTab(this->ui->tab_widget->currentIndex());
        this->do_numbers_of_tabs_right();
    }
}

void EventDialog::on_button_ok_clicked()
{
    this->on_button_apply_clicked();
    this->close();
}

void EventDialog::on_button_cancel_clicked()
{
    this->close();
}

void EventDialog::on_button_apply_clicked()
{
    QJsonArray pages;
    for (int i = 0; i < this->ui->tab_widget->count(); i++)
    {
        pages.append(((EventPage*)this->ui->tab_widget->widget(i))->getPage());
    }
    this->event.insert("@pages", pages);
    this->event.insert("@name", this->ui->line_name->text());
    int id = this->event.value("@id").toInt();
    mc->set_event_by_id(id, this->event);
    this->do_numbers_of_tabs_right();
    emit ok_clicked();
}

void EventDialog::on_button_copy_clicked()
{
    QJsonDocument doc(((EventPage*)this->ui->tab_widget->currentWidget())->getPage());
    QSettings settings;
    settings.setValue("copied_event_page", doc.toJson(QJsonDocument::Compact));
}


void EventDialog::on_button_paste_clicked()
{
    QSettings settings;
    QByteArray json = settings.value("copied_event_page").toByteArray();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(json, &err);
    if (err.error != QJsonParseError::NoError) return;

    this->ui->tab_widget->insertTab(this->ui->tab_widget->currentIndex(), new EventPage(doc.object(), mc), QString());
    this->do_numbers_of_tabs_right();
}

