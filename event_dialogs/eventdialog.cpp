#include "eventdialog.h"
#include "ui_eventdialog.h"

#include "eventpage.h"

EventDialog::EventDialog(QJsonObject event, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventDialog)
{
    ui->setupUi(this);

    Q_UNUSED(event);

    if (this->event != 0)
    {
        //qDebug() << event->pages.length();
        //this->ui->label_event_name->setText(event->name);
        //for (int i = 0; i < this->event->pages.size(); i++)
        {
            //this->ui->tab_widget->addTab(new EventPage(this->ui->tab_widget,this->event->pages.at(i),system), QString::number(i+1));
        }
    }

    this->do_numbers_of_tabs_right();
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

    if (this->ui->tab_widget->count() <= 1) this->ui->button_delete->setEnabled(false);
    else this->ui->button_delete->setEnabled(true);

}

void EventDialog::on_button_new_clicked()
{
    //this->ui->tab_widget->addTab(new EventPage(nullptr, 0, system), "1");
    this->do_numbers_of_tabs_right();
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
    for (int i = 0 ; i < event->pages.length(); i++)
    {
        delete event->pages.at(i);
    }
    event->pages.clear();

    for (int i = 0; i < this->ui->tab_widget->count(); i++)
    {
        event->pages.append(((EventPage*)this->ui->tab_widget->widget(i))->create_new_rpgeventpage());
    }
}
