#include "eventdialog.h"
#include "ui_eventdialog.h"

#include "eventpage.h"

EventDialog::EventDialog(QWidget *parent, RPGEvent *event) :
    QWidget(parent),
    ui(new Ui::EventDialog)
{
    ui->setupUi(this);
    this->event = event;
    this->ui->label_event_name->setText(event->name);
    for (int i = 0; i < this->event->pages.size(); i++)
    {
        this->ui->tab_widget->addTab(new EventPage(nullptr,this->event->pages.at(i)), QString::number(i+1));
    }

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
}

void EventDialog::on_button_new_clicked()
{
    this->ui->tab_widget->addTab(new EventPage, "1");
    this->do_numbers_of_tabs_right();
}

void EventDialog::on_button_delete_clicked()
{
    this->ui->tab_widget->removeTab(this->ui->tab_widget->currentIndex());
    this->do_numbers_of_tabs_right();
}
