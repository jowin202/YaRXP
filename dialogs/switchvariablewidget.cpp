#include "switchvariablewidget.h"
#include "ui_switchvariablewidget.h"

#include "RXIO/RXObjects/rpgsystem.h"

#include "listdialog.h"

SwitchVariableWidget::SwitchVariableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SwitchVariableWidget)
{
    ui->setupUi(this);
}

SwitchVariableWidget::~SwitchVariableWidget()
{
    delete ui;
}

void SwitchVariableWidget::setSwitchWidget(RPGSystem *system)
{
    this->system = system;
    this->switch_widget = true;
    this->ui->comboBox->clear();
    for (int i = 0; i < system->switches_names.length(); i++)
    {
        this->ui->comboBox->addItem(QString("%1: %2").arg(i+1,4,10,QChar('0')).arg(system->switches_names.at(i)));
    }
}

void SwitchVariableWidget::setVariableWidget(RPGSystem *system)
{
    this->system = system;
    this->variable_widget = true;
    this->ui->comboBox->clear();
    for (int i = 0; i < system->variable_names.length(); i++)
    {
        this->ui->comboBox->addItem(QString("%1: %2").arg(i+1,4,10,QChar('0')).arg(system->variable_names.at(i)));
    }
}

int SwitchVariableWidget::getValue()
{
    return this->ui->comboBox->currentIndex()+1;
}

void SwitchVariableWidget::on_toolButton_clicked()
{
    ListDialog *dialog = new ListDialog(system,0);
    if (switch_widget) dialog->switch_dialog();
    else if (variable_widget) dialog->variable_dialog();
    //else ... normal dialog without switch or variable

    dialog->setValue(this->ui->comboBox->currentIndex());
    connect(dialog, SIGNAL(ok_clicked(int)), this, SLOT(setValue_from_dialog(int)));
    dialog->show();
}

void SwitchVariableWidget::setValue_from_dialog(int value)
{
    if (this->switch_widget) this->setSwitchWidget(system); //update combo
    else if (this->variable_widget) this->setVariableWidget(system); //update combo
    //no else
    this->ui->comboBox->setCurrentIndex(value);
}

void SwitchVariableWidget::setValue(int value)
{
    this->ui->comboBox->setCurrentIndex(value-1);
}
