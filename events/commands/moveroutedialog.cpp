#include "moveroutedialog.h"
#include "ui_moveroutedialog.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/factory.h"
#include "RXIO2/rpgmapcontroller.h"

#include "dialogs/audiodialog.h"

MoveRouteDialog::MoveRouteDialog(RPGDB *db, RPGMapController *mc, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MoveRouteDialog)
{
    ui->setupUi(this);
    this->db = db;

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    this->ui->listWidget->setFont(font);

    this->ui->listWidget->setDragDropMode(QAbstractItemView::InternalMove);



    //this->action_delete.setText("Delete");
    this->action_delete.setShortcut(Qt::Key_Delete);
    this->action_delete.setShortcutContext(Qt::WidgetShortcut);
    this->ui->listWidget->addAction(&this->action_delete);
    connect(&this->action_delete, SIGNAL(triggered()), this, SLOT(do_delete()));



    QJsonObject events = mc->current_map()->object().value("@events").toObject();

    //THX to https://stackoverflow.com/questions/54427846/how-can-i-do-a-numeric-sort-of-a-qstringlist-in-qt-4-6-2-where-qcollator-is-not
    QStringList keys = events.keys();
    keys.remove(keys.indexOf("RXClass"));

    QMap<int, QString> m;
    for (auto s : keys) m[s.toInt()] = s;
    keys = QStringList(m.values());

    this->ui->combo_event->addItem("Player", -1);
    this->ui->combo_event->addItem("This event", 0);
    if (parameters.at(0).toInt() == -1)
        this->ui->combo_event->setCurrentIndex(0);
    if (parameters.at(0).toInt() == 0)
        this->ui->combo_event->setCurrentIndex(1);

    for (QString key : keys)
    {
        this->ui->combo_event->addItem(QString("%1: ").arg(key.toInt(), 3,10,QChar('0')) + events.value(key).toObject().value("@name").toString(), key.toInt());
        if (parameters.at(0).toInt() == key.toInt())
            this->ui->combo_event->setCurrentIndex(this->ui->combo_event->count()-1);
    }

    this->ui->check_skippable->setChecked(parameters.at(1).toObject().value("@skippable").toBool());
    this->ui->check_repeat->setChecked(parameters.at(1).toObject().value("@repeat").toBool());

    this->list = parameters.at(1).toObject().value("@list").toArray();
    //remove the last element here
    for (int i = this->list.count()-1; i >= 0; i--)
    {
        if (this->list.at(i).toObject().value("@code").toInt() == 0)
            this->list.removeAt(i);
    }
    this->fill_list();
    this->ui->listWidget->setCurrentRow(this->ui->listWidget->count()-1);
}

MoveRouteDialog::~MoveRouteDialog()
{
    delete ui;
}

void MoveRouteDialog::fill_list()
{
    this->ui->listWidget->clear();
    for (int i = 0; i < this->list.count(); i++)
    {
        QString text;
        int code = this->list.at(i).toObject().value("@code").toInt();
        QJsonArray p = this->list.at(i).toObject().value("@parameters").toArray();
        if (code == 14)
            text += QString("%1, %2").arg(p.at(0).toInt()).arg(p.at(1).toInt());
        else if (code == 15)
            text += QString("%1 frame(s)").arg(p.at(0).toInt());
        else if (code == 27 || code == 28)
            text += QString("%1").arg(p.at(0).toInt(),4,10, QChar('0'));
        else if (code == 29 || code == 30 || code == 42)
            text += QString("%1").arg(p.at(0).toInt());
        else if (code == 41)
            text += QString("'%1', %2, %3, %4").arg(p.at(0).toString()).arg(p.at(1).toInt()).arg(p.at(2).toInt()).arg(p.at(3).toInt());
        else if (code == 43)
            text += this->text_blend.at(p.at(0).toInt());
        else if (code == 44)
            text += QString("'%1', %2, %3").arg(p.at(0).toObject().value("@name").toString()).arg(p.at(0).toObject().value("@volume").toInt()).arg(p.at(0).toObject().value("@pitch").toInt());
        else if (code == 45)
            text += p.at(0).toString();
        this->ui->listWidget->addItem("@>" + this->text_move_routes.at(code) + text);
    }
    this->ui->listWidget->addItem("@>"); //add last element manually;
}

void MoveRouteDialog::on_button_move_down_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(1);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_move_left_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(2);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_move_right_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(3);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_move_up_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(4);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_move_lower_left_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(5);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_move_lower_right_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(6);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_move_upper_left_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(7);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_move_upper_right_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(8);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_move_random_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(9);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_move_toward_player_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(10);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_move_away_from_player_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(11);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_one_step_forward_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(12);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_one_step_backward_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(13);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_jump_clicked()
{
    qDebug() << "not implemented yet";
    exit(3);
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(14);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_wait_clicked()
{
    bool ok;
    int v = QInputDialog::getInt(this, "Wait", "Frames:", 4,0,999, 1, &ok);
    if (ok)
    {
        int row = this->ui->listWidget->currentRow();
        QJsonObject obj = Factory().create_move_command(15);
        QJsonArray p;
        p.append(v);
        obj.insert("@parameters", p);
        this->list.insert(row, obj);
        this->fill_list();
        this->ui->listWidget->setCurrentRow(row+1);
    }
}


void MoveRouteDialog::on_button_turn_down_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(16);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_turn_left_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(17);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_turn_right_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(18);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_turn_up_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(19);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_turn_90_right_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(20);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_turn_90_left_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(21);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_turn_180_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(22);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_turn_90_left_or_right_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(23);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_turn_random_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(24);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_turn_toward_player_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(25);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_turn_away_from_player_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(26);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_switch_on_clicked()
{
//27
}


void MoveRouteDialog::on_button_switch_off_clicked()
{
//28
}


void MoveRouteDialog::on_button_change_speed_clicked()
{
//29
}


void MoveRouteDialog::on_button_change_freq_clicked()
{
//30
}


void MoveRouteDialog::on_button_move_animation_on_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(31);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_move_animation_off_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(32);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_stop_animation_on_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(33);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_stop_animation_off_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(34);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_direction_fix_on_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(35);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_direction_fix_off_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(36);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_through_on_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(37);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_through_off_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(38);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_always_on_top_on_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(39);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_always_on_top_off_clicked()
{
    int row = this->ui->listWidget->currentRow();
    QJsonObject obj = Factory().create_move_command(40);
    this->list.insert(row, obj);
    this->fill_list();
    this->ui->listWidget->setCurrentRow(row+1);
}


void MoveRouteDialog::on_button_change_graphic_clicked()
{
//41
}


void MoveRouteDialog::on_button_opacity_clicked()
{

    bool ok;
    int v = QInputDialog::getInt(this, "Change Opacity", "Opacity:", 255,0,255, 1, &ok);
    if (ok)
    {
        int row = this->ui->listWidget->currentRow();
        QJsonObject obj = Factory().create_move_command(42);
        QJsonArray p;
        p.append(v);
        obj.insert("@parameters", p);
        this->list.insert(row, obj);
        this->fill_list();
        this->ui->listWidget->setCurrentRow(row+1);
    }
}


void MoveRouteDialog::on_button_change_blending_clicked()
{
//43
}


void MoveRouteDialog::on_button_play_se_clicked()
{
    AudioDialog *dialog = new AudioDialog(this->db, "", 100, 100, AudioDialog::SE);
    dialog->show();
    connect(dialog, &AudioDialog::ok_clicked, [=](QString name, int volume, int pitch){
        int row = this->ui->listWidget->currentRow();
        QJsonObject obj = Factory().create_move_command(44);
        QJsonArray p;
        p.append(Factory().create_audiofile(name,volume,pitch));
        obj.insert("@parameters", p);
        this->list.insert(row, obj);
        this->fill_list();
        this->ui->listWidget->setCurrentRow(row+1);
    });
}


void MoveRouteDialog::on_button_script_clicked()
{
    //45
}




void MoveRouteDialog::on_button_cancel_clicked()
{
    this->close();
}


void MoveRouteDialog::on_button_ok_clicked()
{
    this->list.append(Factory().create_move_command(0)); //append a "neutral" command at the end (RPGMaker compatible)
    QJsonArray parameters;
    parameters.append(this->ui->combo_event->currentData().toInt());
    QJsonObject mvrt = Factory().create_move_route(this->ui->check_repeat->isChecked(), this->ui->check_skippable->isChecked());
    mvrt.insert("@list", this->list);
    parameters.append(mvrt);
    emit ok_clicked(parameters);
    this->close();
}

void MoveRouteDialog::do_delete()
{
    int row = this->ui->listWidget->currentRow();
    if (row != this->ui->listWidget->count()-1) //cant delete the last one
    {
        this->list.removeAt(row);
        this->fill_list();
        this->ui->listWidget->setCurrentRow(row);
    }
}

