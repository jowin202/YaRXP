#include "editscripts.h"
#include "ui_editscripts.h"
#include "highlighter.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"

EditScripts::EditScripts(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditScripts)
{
    ui->setupUi(this);

    this->ui->textEdit->setFontFamily("Monospace");
    highlighter = new Highlighter(this->ui->textEdit->document());

    connect(this->ui->textEdit, SIGNAL(textChanged()), this, SLOT(save()));
}

EditScripts::~EditScripts()
{
    delete ui;
    delete highlighter;
}

void EditScripts::setEC(RPGEditorController *ec)
{
    this->ec = ec;
    connect(ec, &RPGEditorController::current_script_changed, [=](){
        QJsonArray array = ec->get_script_by_id(ec->current_script);
        if (array.count() == 3)
        {
            int uncompressed_size = array.at(0).toInt();

            QByteArray buffer;
            buffer = buffer.prepend(uncompressed_size&0xFF);
            uncompressed_size >>= 8;
            buffer = buffer.prepend(uncompressed_size&0xFF);
            uncompressed_size >>= 8;
            buffer = buffer.prepend(uncompressed_size&0xFF);
            uncompressed_size >>= 8;
            buffer = buffer.prepend(uncompressed_size&0xFF);

            this->ui->textEdit->setText(qUncompress(buffer + QByteArray::fromBase64(array.at(2).toVariant().toByteArray())));
        }
    });
}

void EditScripts::save()
{
    QJsonArray array = ec->get_script_by_id(ec->current_script);
    QByteArray data = qCompress(this->ui->textEdit->toPlainText().toUtf8());
    int size = 0;
    size |= (data.at(0)&0xFF);
    size <<= 8;
    size |= (data.at(1)&0xFF);
    size <<= 8;
    size |= (data.at(2)&0xFF);
    size <<= 8;
    size |= (data.at(3)&0xFF);
    data = data.remove(0,4);

    array.removeAt(0);
    array.insert(0,size);

    array.removeAt(2);
    array.insert(2,QString(data.toBase64()));
    ec->set_object_by_id(RPGDB::SCRIPTS,ec->current_script,array);
}


