#include "eventpage.h"
#include "ui_eventpage.h"

EventPage::EventPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventPage)
{
    ui->setupUi(this);
}

EventPage::~EventPage()
{
    delete ui;
}
