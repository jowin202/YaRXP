#include "eventdialog.h"
#include "ui_eventdialog.h"

#include "eventpage.h"

EventDialog::EventDialog(RPGEvent *event, RPGSystem *system, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventDialog)
{
    ui->setupUi(this);

    this->event = event;

    if (this->event != 0)
    {
        //ONLY DO THIS FOR EXISTING EVENTS
        this->ui->label_event_name->setText(event->name);
        for (int i = 0; i < this->event->pages.size(); i++)
        {
            this->ui->tab_widget->addTab(new EventPage(this->ui->tab_widget,this->event->pages.at(i),system), QString::number(i+1));
        }
    }

}

EventDialog::~EventDialog()
{
    delete ui;
}

void EventDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    this->deleteLater();
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
    //TODO event page
    this->ui->tab_widget->removeTab(this->ui->tab_widget->currentIndex());
    this->do_numbers_of_tabs_right();
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

}
