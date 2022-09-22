#include "editexpcurve.h"
#include "ui_editexpcurve.h"

#include <QScrollBar>
#include <QDebug>
#include <QtMath>
#include <QSlider>

EditExpCurve::EditExpCurve(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditExpCurve)
{
    ui->setupUi(this);
    connect(this->ui->spin_basis, SIGNAL(valueChanged(int)), this, SLOT(update_levels()));
    connect(this->ui->spin_inflation, SIGNAL(valueChanged(int)), this, SLOT(update_levels()));

    this->update_levels();
}

EditExpCurve::~EditExpCurve()
{
    delete ui;
}

void EditExpCurve::set_values(int basis, int inflation)
{
    this->ui->spin_basis->setValue(basis);
    this->ui->spin_inflation->setValue(inflation);
}

void EditExpCurve::update_levels()
{
    int basis = this->ui->spin_basis->value();
    int inflation = this->ui->spin_inflation->value();
    int total = 0;

    this->ui->text_next_lv->clear();
    this->ui->text_total->clear();

    this->ui->text_total->append("L 01:  <font color=\"red\">0</font>");


    qreal pow = 2.4 + inflation/100.0;
    for (int i = 2; i <= 99; i++)
    {
        qreal val = basis * qPow((i + 3),pow) / (qPow(5,pow));
        total += val;
        this->ui->text_next_lv->append("L " + QString::number(i-1).rightJustified(2,'0') + ":  <font color=\"green\">" + QString::number((int)val).rightJustified(7,' ') + "</font>");
        this->ui->text_total->append("L " + QString::number(i).rightJustified(2,'0')  + ":  <font color=\"red\">" + QString::number((int)total).rightJustified(7,' ') + "</font>");
    }
    this->ui->text_next_lv->verticalScrollBar()->setValue(0);
    this->ui->text_total->verticalScrollBar()->setValue(0);
}

void EditExpCurve::on_button_ok_clicked()
{
    emit exp_curve_set(this->ui->spin_basis->value(), this->ui->spin_inflation->value());
    this->close();
}

void EditExpCurve::on_button_cancel_clicked()
{
    this->close();
}
