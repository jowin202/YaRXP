#include "showtextdialog.h"
#include "ui_showtextdialog.h"

ShowTextDialog::ShowTextDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowTextDialog)
{
    ui->setupUi(this);
}

ShowTextDialog::~ShowTextDialog()
{
    delete ui;
}

void ShowTextDialog::setString(QString text)
{
    this->ui->plainTextEdit->setPlainText(text);
}


void ShowTextDialog::on_button_ok_clicked()
{
    emit ok_clicked(this->ui->plainTextEdit->toPlainText());
    this->close();
}

void ShowTextDialog::on_button_cancel_clicked()
{
    this->close();
}
