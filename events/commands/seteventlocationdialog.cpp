#include "seteventlocationdialog.h"
#include "ui_seteventlocationdialog.h"

#include "dialogs/maplocationfinderdialog.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgmapcontroller.h"
#include "RXIO2/rpgmapinfocontroller.h"


SetEventLocationDialog::SetEventLocationDialog(RPGDB *db, RPGMapController *mc, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetEventLocationDialog)
{
    ui->setupUi(this);
    this->db = db;
    this->mc = mc;
    this->ui->widget_var_x->setVariableWidget(db);
    this->ui->widget_var_y->setVariableWidget(db);

    QJsonObject events = mc->current_map()->object().value("@events").toObject();

    //THX to https://stackoverflow.com/questions/54427846/how-can-i-do-a-numeric-sort-of-a-qstringlist-in-qt-4-6-2-where-qcollator-is-not
    QStringList keys = events.keys();
    keys.remove(keys.indexOf("RXClass"));

    QMap<int, QString> m;
    for (auto s : keys) m[s.toInt()] = s;
    keys = QStringList(m.values());

    this->ui->combo_event->addItem("This event", 0);
    for (QString key : keys)
    {
        this->ui->combo_event->addItem(QString("%1: ").arg(key.toInt(), 3,10,QChar('0')) + events.value(key).toObject().value("@name").toString(), key.toInt());
        this->ui->combo_exchange_event->addItem(QString("%1: ").arg(key.toInt(), 3,10,QChar('0')) + events.value(key).toObject().value("@name").toString(), key.toInt());
        if (parameters.at(0).toInt() == key.toInt())
            this->ui->combo_event->setCurrentIndex(this->ui->combo_event->count()-1);
    }

    if (parameters.at(1).toInt() == 0)
    {
        this->ui->radio_direct->setChecked(true);
        this->set_direct(parameters.at(2).toInt(), parameters.at(3).toInt());
    }
    else if (parameters.at(1).toInt() == 1)
    {
        this->ui->radio_variable->setChecked(true);
        this->ui->widget_var_x->setValue(parameters.at(2).toInt());
        this->ui->widget_var_y->setValue(parameters.at(3).toInt());
    }
    else if (parameters.at(1).toInt() == 2)
    {
        this->ui->radio_change->setChecked(true);
        for (int i = 0; i < this->ui->combo_exchange_event->count(); i++)
        {
            if (this->ui->combo_exchange_event->itemData(i).toInt() == parameters.at(2).toInt())
                this->ui->combo_exchange_event->setCurrentIndex(i);
        }
    }
    this->ui->combo_direction->setCurrentIndex(parameters.at(4).toInt()/2);
}

SetEventLocationDialog::~SetEventLocationDialog()
{
    delete ui;
}

void SetEventLocationDialog::set_direct(int x, int y)
{
    RPGMapInfoController mic(db);
    this->x = x;
    this->y = y;
    this->ui->line_direct->setText(QString("(%1, %2)").arg(x,3,10,QChar('0')).arg(y,3,10,QChar('0')));
}

void SetEventLocationDialog::on_button_ok_clicked()
{
    QJsonArray p;
    p.append(this->ui->combo_event->currentData().toInt());
    if (this->ui->radio_direct->isChecked())
    {
        p.append(0);
        p.append(x);
        p.append(y);
    }
    else if (this->ui->radio_variable->isChecked())
    {
        p.append(1);
        p.append(this->ui->widget_var_x->getValue());
        p.append(this->ui->widget_var_y->getValue());
    }
    else if (this->ui->radio_change->isChecked())
    {
        p.append(2);
        p.append(this->ui->combo_exchange_event->currentData().toInt());
        p.append(0);
    }

    p.append(this->ui->combo_direction->currentIndex()*2);

    this->close();
    emit ok_clicked(p);
}


void SetEventLocationDialog::on_button_cancel_clicked()
{
    this->close();
}


void SetEventLocationDialog::on_button_change_location_clicked()
{
    MapLocationFinderDialog *dialog = new MapLocationFinderDialog(db,mc->get_current_map_id(),x,y, false);
    dialog->show();
    connect(dialog, &MapLocationFinderDialog::ok_clicked, [=](int id, int x, int y){
        Q_UNUSED(id);
       this->set_direct(x,y);
    });
}

