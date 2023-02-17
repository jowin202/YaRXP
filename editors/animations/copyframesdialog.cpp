#include "copyframesdialog.h"
#include "ui_copyframesdialog.h"

CopyFramesDialog::CopyFramesDialog(int max_frames, bool true_for_copy_false_for_clear, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CopyFramesDialog)
{
    ui->setupUi(this);
    this->ui->spin_from->setMaximum(max_frames);
    this->ui->spin_to->setMaximum(max_frames);
    this->ui->spin_dest->setMaximum(max_frames);
    this->ui->spin_to->setValue(max_frames);

    if (!true_for_copy_false_for_clear)
    {
        this->setWindowTitle("Clear Frames");
        this->ui->label_2->setVisible(false);
        this->ui->spin_dest->setVisible(false);
        this->ui->label_to->setVisible(false);
    }

    connect(this->ui->spin_dest, SIGNAL(valueChanged(int)), this,  SLOT(update()));
    connect(this->ui->spin_from, SIGNAL(valueChanged(int)), this,  SLOT(update()));
    connect(this->ui->spin_to, SIGNAL(valueChanged(int)), this,  SLOT(update()));
    this->update();
}

CopyFramesDialog::~CopyFramesDialog()
{
    delete ui;
}

void CopyFramesDialog::update()
{
    int delta = qAbs(this->ui->spin_to->value()-this->ui->spin_from->value());
    this->ui->label_to->setText("to " + QString::number(this->ui->spin_dest->value()+delta));
}

void CopyFramesDialog::on_button_ok_clicked()
{
    this->close();
    emit ok_clicked(this->ui->spin_from->value(), this->ui->spin_to->value(), this->ui->spin_dest->value());
}


void CopyFramesDialog::on_button_cancel_clicked()
{
    this->close();
}

