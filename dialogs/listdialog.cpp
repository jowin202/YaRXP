#include "listdialog.h"
#include "ui_listdialog.h"

#include "RXIO/RXObjects/rpgsystem.h"

ListDialog::ListDialog(RPGSystem *system, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListDialog)
{
    ui->setupUi(this);
    this->system = system;
}

ListDialog::~ListDialog()
{
    delete ui;
}

void ListDialog::switch_dialog()
{
    this->switch_is_set = true;
    this->setWindowTitle("YaRXP - Switches");
    this->ui->list->clear();
    for (int i = 0; i < system->switches_names.length(); i++)
        this->ui->list->addItem(QString("%1: %2").arg(i+1,4,10,QChar('0')).arg(system->switches_names.at(i)));
}

void ListDialog::variable_dialog()
{
    this->variable_is_set = true;
    this->setWindowTitle("YaRXP - Variables");
    this->ui->list->clear();
    for (int i = 0; i < system->variable_names.length(); i++)
        this->ui->list->addItem(QString("%1: %2").arg(i+1,4,10,QChar('0')).arg(system->variable_names.at(i)));
}

int ListDialog::getValue()
{
    return this->ui->list->currentRow();
}

void ListDialog::setValue(int value)
{
    this->ui->list->setCurrentRow(value);
}

void ListDialog::on_button_cancel_clicked()
{
    this->close();
}

void ListDialog::on_button_ok_clicked()
{
    emit ok_clicked(this->getValue());
    this->close();
}

void ListDialog::on_button_change_max_clicked()
{
    if (switch_is_set)
    {
        int val = QInputDialog::getInt(this, "Set Max Number", "Set Max Number:", system->switches_names.length(),1,9999);
        while (system->switches_names.length() > val) { system->switches_names.pop_back(); }
        while (system->switches_names.length() < val) { system->switches_names.append(RPGString()); }

        this->switch_dialog();
    }
    else if (variable_is_set)
    {
        int val = QInputDialog::getInt(this, "Set Max Number", "Set Max Number:", system->variable_names.length(),1,9999);
        while (system->variable_names.length() > val) { system->variable_names.pop_back(); }
        while (system->variable_names.length() < val) { system->variable_names.append(RPGString()); }

        this->variable_dialog();
    }
}

void ListDialog::on_list_doubleClicked(const QModelIndex &index)
{
    int n = index.row();

    if (switch_is_set)
    {
        QString name = QInputDialog::getText(this, "Rename Switch", "Rename Switch", QLineEdit::Normal, system->switches_names[n]);
        system->switches_names[n] = name;
        this->switch_dialog();
    }
    else if (variable_is_set)
    {
        QString name = QInputDialog::getText(this, "Rename Variable", "Rename Variable", QLineEdit::Normal, system->variable_names[n]);
        system->variable_names[n] = name;
        this->variable_dialog();
    }
    this->ui->list->setCurrentRow(n);
}
