#include "combodialog.h"
#include "ui_combodialog.h"


ComboDialog::ComboDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComboDialog)
{
    ui->setupUi(this);
}

ComboDialog::~ComboDialog()
{
    delete ui;
}

void ComboDialog::setText(QString win_title, QString text)
{
    this->setWindowTitle(win_title);
    this->ui->text->setText(text);
}

QComboBox *ComboDialog::combo()
{
    return this->ui->comboBox;
}


void ComboDialog::on_button_ok_clicked()
{
    emit this->ok_clicked(this->ui->comboBox->currentData().toInt());
    this->close();
}

void ComboDialog::on_button_cancel_clicked()
{
    this->close();
}
