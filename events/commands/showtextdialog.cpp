#include "showtextdialog.h"
#include "ui_showtextdialog.h"

ShowTextDialog::ShowTextDialog(QWidget *parent, int code) :
    QWidget(parent),
    ui(new Ui::ShowTextDialog)
{
    ui->setupUi(this);
    if (code == 108 || code == 408)
    {
        this->setWindowTitle("Comment");
    }
    else if (code == 355 || code == 655)
    {
        QFont font;
        font.setFamily("Monospace");
        this->ui->plainTextEdit->setFont(font);
        this->setWindowTitle("Script");
    }
    this->ui->plainTextEdit->setFocus();
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
    this->close();
    emit ok_clicked(this->ui->plainTextEdit->toPlainText());
}

void ShowTextDialog::on_button_cancel_clicked()
{
    this->close();
}
