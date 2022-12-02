#include "switchvariablewidget.h"
#include "ui_switchvariablewidget.h"

#include "RXIO2/rpgdb.h"

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

void SwitchVariableWidget::setSwitchWidget(RPGDB *db)
{
    this->db = db;
    this->switch_widget = true;
    this->ui->comboBox->clear();
    for (int i = 1; i < db->get_switch_names().count(); i++)
    {
        this->ui->comboBox->addItem(QString("%1: %2").arg(i,4,10,QChar('0')).arg(db->get_switch_names().at(i).toString()));
    }
}

void SwitchVariableWidget::setVariableWidget(RPGDB *db)
{
    this->db = db;
    this->variable_widget = true;
    this->ui->comboBox->clear();
    for (int i = 1; i < db->get_variable_names().count(); i++)
    {
        this->ui->comboBox->addItem(QString("%1: %2").arg(i,4,10,QChar('0')).arg(db->get_variable_names().at(i).toString()));
    }
}

int SwitchVariableWidget::getValue()
{
    return this->ui->comboBox->currentIndex()+1;
}

void SwitchVariableWidget::on_toolButton_clicked()
{
    ListDialog *dialog = new ListDialog(db,0);
    if (switch_widget) dialog->switch_dialog();
    else if (variable_widget) dialog->variable_dialog();
    //else ... normal dialog without switch or variable

    dialog->setValue(this->ui->comboBox->currentIndex());
    connect(dialog, SIGNAL(ok_clicked(int)), this, SLOT(setValue_from_dialog(int)));
    dialog->show();
}

void SwitchVariableWidget::setValue_from_dialog(int value)
{
    if (this->switch_widget) this->setSwitchWidget(db); //update combo
    else if (this->variable_widget) this->setVariableWidget(db); //update combo
    //no else
    this->ui->comboBox->setCurrentIndex(value);
}

void SwitchVariableWidget::setValue(int value)
{
    this->ui->comboBox->setCurrentIndex(value-1);
}
