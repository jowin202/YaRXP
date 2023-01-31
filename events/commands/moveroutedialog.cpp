#include "moveroutedialog.h"
#include "ui_moveroutedialog.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/factory.h"
#include "RXIO2/rpgmapcontroller.h"

#include "spinspindialog.h"
#include "dialogs/audiodialog.h"
#include "dialogs/imagechooserdialog.h"
#include "dialogs/listdialog.h"
#include "dialogs/combodialog.h"


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
    connect(this->ui->listWidget->model(), &QAbstractItemModel::rowsMoved, [=](const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationRow)
    {
        Q_UNUSED(sourceParent);
        Q_UNUSED(destinationParent);
        if (sourceStart != sourceEnd || sourceStart == this->ui->listWidget->count()-1 || destinationRow == this->ui->listWidget->count())
        { //only single move and last row cannot be moved
            this->fill_list(); //restore
            return; //single selection
        }


        QJsonObject obj = list.at(sourceStart).toObject();
        if (sourceStart < destinationRow)
        {
            list.insert(destinationRow, obj);
            list.removeAt(sourceStart);
        }
        else if (sourceStart > destinationRow)
        {
            list.removeAt(sourceStart);
            list.insert(destinationRow,obj);
        }
    });

    this->action_delete.setShortcut(Qt::Key_Delete);
    this->action_delete.setShortcutContext(Qt::WidgetShortcut);
    this->ui->listWidget->addAction(&this->action_delete);
    connect(&this->action_delete, SIGNAL(triggered()), this, SLOT(do_delete()));




    if (parameters.at(0).toInt() == -404) //from event page. no event needed
    {
        this->ui->combo_event->setEnabled(false);
    }
    else
    {
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
    QJsonArray a;
    a.append(0);
    a.append(0);
    SpinSpinDialog *dialog = new SpinSpinDialog(14,a);
    dialog->show();
    connect(dialog, &SpinSpinDialog::ok_clicked, [=](QJsonArray p)
    {
        int row = this->ui->listWidget->currentRow();
        QJsonObject obj = Factory().create_move_command(14);
        obj.insert("@parameters", p);
        this->list.insert(row, obj);
        this->fill_list();
        this->ui->listWidget->setCurrentRow(row+1);
    });
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
    ListDialog *dialog = new ListDialog(db,0);
    dialog->switch_dialog();
    //dialog->setValue( current );
    dialog->show();
    connect(dialog, &ListDialog::ok_clicked, [=](int v)
    {
        QJsonArray a;
        a.append(v+1);

        int row = this->ui->listWidget->currentRow();
        QJsonObject obj = Factory().create_move_command(27);
        obj.insert("@parameters", a);
        this->list.insert(row, obj);
        this->fill_list();
        this->ui->listWidget->setCurrentRow(row+1);
    });
}


void MoveRouteDialog::on_button_switch_off_clicked()
{
    ListDialog *dialog = new ListDialog(db,0);
    dialog->switch_dialog();
    //dialog->setValue( current );
    dialog->show();
    connect(dialog, &ListDialog::ok_clicked, [=](int v)
    {
        QJsonArray a;
        a.append(v+1);

        int row = this->ui->listWidget->currentRow();
        QJsonObject obj = Factory().create_move_command(28);
        obj.insert("@parameters", a);
        this->list.insert(row, obj);
        this->fill_list();
        this->ui->listWidget->setCurrentRow(row+1);
    });
}


void MoveRouteDialog::on_button_change_speed_clicked()
{
    //29
    ComboDialog *dialog = new ComboDialog;
    dialog->setText("Change Speed", "Speed:");
    dialog->combo()->addItem("1: Slowest", 1);
    dialog->combo()->addItem("2: Slower", 2);
    dialog->combo()->addItem("3: Slow", 3);
    dialog->combo()->addItem("4: Fast", 4);
    dialog->combo()->addItem("5: Faster", 5);
    dialog->combo()->addItem("6: Fastest", 6);
    dialog->combo()->setCurrentIndex(2);
    dialog->show();

    connect(dialog, &ComboDialog::ok_clicked, [=](int v){
        int row = this->ui->listWidget->currentRow();
        QJsonObject obj = Factory().create_move_command(29);
        QJsonArray p;
        p.append(v);
        obj.insert("@parameters", p);
        this->list.insert(row, obj);
        this->fill_list();
        this->ui->listWidget->setCurrentRow(row+1);
    });
}


void MoveRouteDialog::on_button_change_freq_clicked()
{
    //30
    ComboDialog *dialog = new ComboDialog;
    dialog->setText("Change Freq", "Frequency:");
    dialog->combo()->addItem("1: Lowest", 1);
    dialog->combo()->addItem("2: Lower", 2);
    dialog->combo()->addItem("3: Low", 3);
    dialog->combo()->addItem("4: High", 4);
    dialog->combo()->addItem("5: Higher", 5);
    dialog->combo()->addItem("6: Highest", 6);
    dialog->combo()->setCurrentIndex(2);
    dialog->show();

    connect(dialog, &ComboDialog::ok_clicked, [=](int v){
        int row = this->ui->listWidget->currentRow();
        QJsonObject obj = Factory().create_move_command(30);
        QJsonArray p;
        p.append(v);
        obj.insert("@parameters", p);
        this->list.insert(row, obj);
        this->fill_list();
        this->ui->listWidget->setCurrentRow(row+1);
    });
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
    ImageChooserDialog *dialog = new ImageChooserDialog(db, "", 0, 0, 2, 255, 0, 0, 0);
    dialog->show();

    connect(dialog, &ImageChooserDialog::ok_clicked, [=](QString name, int hue, int pattern, int direction, int opacity, int blend, int tile_id)
    {
        QJsonArray p;
        p.append(name);
        p.append(hue);
        p.append(direction);
        p.append(pattern);
        Q_UNUSED(opacity);
        Q_UNUSED(blend);
        Q_UNUSED(tile_id);

        int row = this->ui->listWidget->currentRow();
        QJsonObject obj = Factory().create_move_command(41);
        obj.insert("@parameters", p);
        this->list.insert(row, obj);
        this->fill_list();
        this->ui->listWidget->setCurrentRow(row+1);
    });
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
    ComboDialog *dialog = new ComboDialog;
    dialog->setText("Change Blending", "Blending:");
    dialog->combo()->addItem("Normal", 0);
    dialog->combo()->addItem("Add", 1);
    dialog->combo()->addItem("Sub", 2);
    dialog->combo()->setCurrentIndex(0);
    dialog->show();

    connect(dialog, &ComboDialog::ok_clicked, [=](int v){
        int row = this->ui->listWidget->currentRow();
        QJsonObject obj = Factory().create_move_command(43);
        QJsonArray p;
        p.append(v);
        obj.insert("@parameters", p);
        this->list.insert(row, obj);
        this->fill_list();
        this->ui->listWidget->setCurrentRow(row+1);
    });
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
    bool ok;
    QString script = QInputDialog::getText(this, "Script", "",QLineEdit::Normal, "", &ok);
    if (ok)
    {
        int row = this->ui->listWidget->currentRow();
        QJsonObject obj = Factory().create_move_command(45);
        QJsonArray p;
        p.append(script);
        obj.insert("@parameters", p);
        this->list.insert(row, obj);
        this->fill_list();
        this->ui->listWidget->setCurrentRow(row+1);
    }
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
    this->close();
    emit ok_clicked(parameters);
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


void MoveRouteDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    int row = this->ui->listWidget->indexFromItem(item).row();
    int code = this->list.at(row).toObject().value("@code").toInt();

    if (code == 14)
    {
        QJsonArray a = this->list.at(row).toObject().value("@parameters").toArray();
        SpinSpinDialog *dialog = new SpinSpinDialog(14,a);
        dialog->show();
        connect(dialog, &SpinSpinDialog::ok_clicked, [=](QJsonArray p)
        {
            QJsonObject obj = this->list.at(row).toObject();
            obj.insert("@parameters", p);
            this->list.removeAt(row);
            this->list.insert(row, obj);
            this->fill_list();
            this->ui->listWidget->setCurrentRow(row);
        });
    }
    else if (code == 15)
    {
        QJsonArray a = this->list.at(row).toObject().value("@parameters").toArray();
        bool ok;
        int v = QInputDialog::getInt(this, "Wait", "Frames:", a.at(0).toInt(),0,999, 1, &ok);
        if (ok)
        {
            QJsonArray p;
            p.append(v);
            QJsonObject obj = this->list.at(row).toObject();
            obj.insert("@parameters", p);
            this->list.removeAt(row);
            this->list.insert(row, obj);
            this->fill_list();
            this->ui->listWidget->setCurrentRow(row);
        }
    }
    else if (code == 27)
    {
        QJsonArray a = this->list.at(row).toObject().value("@parameters").toArray();
        ListDialog *dialog = new ListDialog(db,0);
        dialog->switch_dialog();
        dialog->setValue( a.at(0).toInt()-1 );
        dialog->show();
        connect(dialog, &ListDialog::ok_clicked, [=](int v)
        {
            QJsonArray a;
            a.append(v+1);

            QJsonObject obj = this->list.at(row).toObject();
            obj.insert("@parameters", a);
            this->list.removeAt(row);
            this->list.insert(row, obj);
            this->fill_list();
            this->ui->listWidget->setCurrentRow(row);
        });
    }
    else if(code == 28)
    {
        QJsonArray a = this->list.at(row).toObject().value("@parameters").toArray();
        ListDialog *dialog = new ListDialog(db,0);
        dialog->switch_dialog();
        dialog->setValue( a.at(0).toInt()-1 );
        dialog->show();
        connect(dialog, &ListDialog::ok_clicked, [=](int v)
        {
            QJsonArray a;
            a.append(v+1);

            QJsonObject obj = this->list.at(row).toObject();
            obj.insert("@parameters", a);
            this->list.removeAt(row);
            this->list.insert(row, obj);
            this->fill_list();
            this->ui->listWidget->setCurrentRow(row);
        });
    }
    else if (code == 29)
    {
        QJsonArray a = this->list.at(row).toObject().value("@parameters").toArray();
        ComboDialog *dialog = new ComboDialog;
        dialog->setText("Change Speed", "Speed:");
        dialog->combo()->addItem("1: Slowest", 1);
        dialog->combo()->addItem("2: Slower", 2);
        dialog->combo()->addItem("3: Slow", 3);
        dialog->combo()->addItem("4: Fast", 4);
        dialog->combo()->addItem("5: Faster", 5);
        dialog->combo()->addItem("6: Fastest", 6);
        dialog->combo()->setCurrentIndex(a.at(0).toInt()-1);
        dialog->show();

        connect(dialog, &ComboDialog::ok_clicked, [=](int v){
            int row = this->ui->listWidget->currentRow();
            QJsonObject obj = this->list.at(row).toObject();
            QJsonArray p;
            p.append(v);
            obj.insert("@parameters", p);
            this->list.removeAt(row);
            this->list.insert(row, obj);
            this->fill_list();
            this->ui->listWidget->setCurrentRow(row);
        });
    }
    else if (code == 30)
    {
        QJsonArray a = this->list.at(row).toObject().value("@parameters").toArray();
        ComboDialog *dialog = new ComboDialog;
        dialog->setText("Change Freq", "Frequency:");
        dialog->combo()->addItem("1: Lowest", 1);
        dialog->combo()->addItem("2: Lower", 2);
        dialog->combo()->addItem("3: Low", 3);
        dialog->combo()->addItem("4: High", 4);
        dialog->combo()->addItem("5: Higher", 5);
        dialog->combo()->addItem("6: Highest", 6);
        dialog->combo()->setCurrentIndex(a.at(0).toInt()-1);
        dialog->show();

        connect(dialog, &ComboDialog::ok_clicked, [=](int v){
            int row = this->ui->listWidget->currentRow();
            QJsonObject obj = this->list.at(row).toObject();
            QJsonArray p;
            p.append(v);
            obj.insert("@parameters", p);
            this->list.removeAt(row);
            this->list.insert(row, obj);
            this->fill_list();
            this->ui->listWidget->setCurrentRow(row);
        });
    }
    else if (code == 41)
    {
        QJsonArray a = this->list.at(row).toObject().value("@parameters").toArray();
        ImageChooserDialog *dialog = new ImageChooserDialog(db, a.at(0).toString(), a.at(1).toInt(), a.at(3).toInt(), a.at(2).toInt(), 255, 0, 0, 0);
        dialog->show();

        connect(dialog, &ImageChooserDialog::ok_clicked, [=](QString name, int hue, int pattern, int direction, int opacity, int blend, int tile_id)
        {
            QJsonArray p;
            p.append(name);
            p.append(hue);
            p.append(direction);
            p.append(pattern);
            Q_UNUSED(opacity);
            Q_UNUSED(blend);
            Q_UNUSED(tile_id);

            int row = this->ui->listWidget->currentRow();
            QJsonObject obj = Factory().create_move_command(41);
            obj.insert("@parameters", p);
            this->list.removeAt(row);
            this->list.insert(row, obj);
            this->fill_list();
            this->ui->listWidget->setCurrentRow(row);
        });
    }
    else if (code == 42)
    {
        QJsonArray a = this->list.at(row).toObject().value("@parameters").toArray();
        bool ok;
        int v = QInputDialog::getInt(this, "Change Opacity", "Opacity:", a.at(0).toInt(),0,255, 1, &ok);
        if (ok)
        {
            QJsonObject obj = this->list.at(row).toObject();
            QJsonArray p;
            p.append(v);
            obj.insert("@parameters", p);
            this->list.removeAt(row);
            this->list.insert(row, obj);
            this->fill_list();
            this->ui->listWidget->setCurrentRow(row);
        }
    }
    else if (code == 43)
    {
        QJsonArray a = this->list.at(row).toObject().value("@parameters").toArray();
        ComboDialog *dialog = new ComboDialog;
        dialog->setText("Change Blending", "Blending:");
        dialog->combo()->addItem("Normal", 0);
        dialog->combo()->addItem("Add", 1);
        dialog->combo()->addItem("Sub", 2);
        dialog->combo()->setCurrentIndex(a.at(0).toInt());
        dialog->show();

        connect(dialog, &ComboDialog::ok_clicked, [=](int v){
            int row = this->ui->listWidget->currentRow();
            QJsonObject obj = this->list.at(row).toObject();
            QJsonArray p;
            p.append(v);
            obj.insert("@parameters", p);
            this->list.removeAt(row);
            this->list.insert(row, obj);
            this->fill_list();
            this->ui->listWidget->setCurrentRow(row);
        });
    }
    else if (code == 44)
    {
        QJsonArray a = this->list.at(row).toObject().value("@parameters").toArray();
        AudioDialog *dialog = new AudioDialog(this->db, a.at(0).toObject().value("@name").toString(), a.at(0).toObject().value("@volume").toInt(), a.at(0).toObject().value("@pitch").toInt(), AudioDialog::SE);
        dialog->show();
        connect(dialog, &AudioDialog::ok_clicked, [=](QString name, int volume, int pitch){
            QJsonObject obj = this->list.at(row).toObject();
            QJsonArray p;
            p.append(Factory().create_audiofile(name,volume,pitch));
            obj.insert("@parameters", p);
            this->list.removeAt(row);
            this->list.insert(row, obj);
            this->fill_list();
            this->ui->listWidget->setCurrentRow(row);
        });
    }
    else if (code == 45)
    {
        QJsonArray a = this->list.at(row).toObject().value("@parameters").toArray();
        bool ok;
        QString script = QInputDialog::getText(this, "Script", "",QLineEdit::Normal, a.at(0).toString(), &ok);
        if (ok)
        {
            QJsonObject obj = this->list.at(row).toObject();
            QJsonArray p;
            p.append(script);
            obj.insert("@parameters", p);
            this->list.removeAt(row);
            this->list.insert(row, obj);
            this->fill_list();
            this->ui->listWidget->setCurrentRow(row);
        }
    }
}

