#include "listdialog.h"
#include "ui_listdialog.h"

#include "RXIO2/rpgdb.h"

ListDialog::ListDialog(RPGDB *db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListDialog)
{
    ui->setupUi(this);
    this->db = db;
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
    for (int i = 1; i < db->get_switch_names().count(); i++)
        this->ui->list->addItem(QString("%1: %2").arg(i,4,10,QChar('0')).arg(db->get_switch_names().at(i).toString()));
}

void ListDialog::variable_dialog()
{
    this->variable_is_set = true;
    this->setWindowTitle("YaRXP - Variables");
    this->ui->list->clear();
    for (int i = 1; i < db->get_variable_names().count(); i++)
        this->ui->list->addItem(QString("%1: %2").arg(i,4,10,QChar('0')).arg(db->get_variable_names().at(i).toString()));
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
        int val = QInputDialog::getInt(this, "Set Max Number", "Set Max Number:", db->get_switch_names().count()-1,1,9999);
        db->set_max_switches(val);
        this->switch_dialog();
    }
    else if (variable_is_set)
    {
        int val = QInputDialog::getInt(this, "Set Max Number", "Set Max Number:", db->get_variable_names().count()-1,1,9999);
        db->set_max_variables(val);
        this->variable_dialog();
    }
}

void ListDialog::on_list_doubleClicked(const QModelIndex &index)
{
    int n = index.row();
    bool ok;

    if (switch_is_set)
    {
        QString name = QInputDialog::getText(this, "Rename Switch", "Rename Switch", QLineEdit::Normal, db->get_switch_names().at(n+1).toString(), &ok);
        if (ok)
            db->change_switch_name(n+1, name);
        this->switch_dialog();
    }
    else if (variable_is_set)
    {
        QString name = QInputDialog::getText(this, "Rename Variable", "Rename Variable", QLineEdit::Normal, db->get_variable_names().at(n+1).toString(), &ok);
        if (ok)
            db->change_variable_name(n+1, name);
        this->variable_dialog();
    }
    this->ui->list->setCurrentRow(n);

}
