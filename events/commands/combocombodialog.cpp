#include "combocombodialog.h"
#include "ui_combocombodialog.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgmapcontroller.h"

ComboComboDialog::ComboComboDialog(RPGDB *db, RPGMapController *mc, int code, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComboComboDialog)
{
    ui->setupUi(this);

    this->code = code;
    if (code == 207)
    {
        this->setWindowTitle("Show Animation");
        this->ui->label->setText("Character:");
        this->ui->label_2->setText("Animation:");

        this->ui->comboBox->addItem("Player", -1);
        this->ui->comboBox->addItem("This event", 0);

        if (parameters.at(0).toInt() == 0)
            this->ui->comboBox->setCurrentIndex(1);

        QJsonObject events = mc->current_map()->object().value("@events").toObject();


        //THX to https://stackoverflow.com/questions/54427846/how-can-i-do-a-numeric-sort-of-a-qstringlist-in-qt-4-6-2-where-qcollator-is-not
        QStringList keys = events.keys();
        keys.remove(keys.indexOf("RXClass"));

        QMap<int, QString> m;
        for (auto s : keys) m[s.toInt()] = s;
        keys = QStringList(m.values());

        for (QString key : keys)
        {
            this->ui->comboBox->addItem(QString("%1: ").arg(key.toInt(), 3,10,QChar('0')) + events.value(key).toObject().value("@name").toString(), key.toInt());
            if (parameters.at(0).toInt() == key.toInt())
                this->ui->comboBox->setCurrentIndex(this->ui->comboBox->count()-1);
        }


        db->fill_combo(this->ui->comboBox_2, RPGDB::ANIMATIONS, true, 3);
        this->ui->comboBox_2->setCurrentIndex(parameters.at(1).toInt()-1);

    }
    else if (code == 321)
    {
        this->setWindowTitle("Change Actor Class");
        this->ui->label->setText("Actor:");
        this->ui->label_2->setText("Class:");

        db->fill_combo(this->ui->comboBox, RPGDB::ACTORS, true, 3);
        db->fill_combo(this->ui->comboBox_2, RPGDB::CLASSES, true, 3);

        this->ui->comboBox->setCurrentIndex(parameters.at(0).toInt()-1);
        this->ui->comboBox_2->setCurrentIndex(parameters.at(1).toInt()-1);
    }
    else if (code == 336)
    {
        this->setWindowTitle("Enemy Transformation");
        this->ui->label->setText("Enemy:");
        this->ui->label_2->setText("Transform to:");

        for (int i = 0; i < 8; i++)
            this->ui->comboBox->addItem(QString("%1.").arg(i+1), i);
        db->fill_combo(this->ui->comboBox_2, RPGDB::ENEMIES, true, 3);

        this->ui->comboBox->setCurrentIndex(parameters.at(0).toInt());
        this->ui->comboBox_2->setCurrentIndex(parameters.at(1).toInt()-1);

    }
}

ComboComboDialog::~ComboComboDialog()
{
    delete ui;
}

void ComboComboDialog::on_button_cancel_clicked()
{
    this->close();
}


void ComboComboDialog::on_button_ok_clicked()
{
    QJsonArray p;


    if (code == 207)
    {
        p.append(this->ui->comboBox->currentData().toInt());
        p.append(this->ui->comboBox_2->currentIndex()+1);
    }
    else if (code == 321)
    {
        p.append(this->ui->comboBox->currentIndex()+1);
        p.append(this->ui->comboBox_2->currentIndex()+1);
    }
    else if (code == 336)
    {
        p.append(this->ui->comboBox->currentIndex());
        p.append(this->ui->comboBox_2->currentIndex()+1);
    }

    emit ok_clicked(p);
    this->close();
}

