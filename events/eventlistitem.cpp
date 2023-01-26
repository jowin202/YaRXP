#include "eventlistitem.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/factory.h"
#include "RXIO2/rpgmapcontroller.h"
#include "RXIO2/rpgmapinfocontroller.h"

#include "commands/changetextoptionsdialog.h"
#include "commands/showtextdialog.h"
#include "commands/radiodialog.h"
#include "commands/singlecombodialog.h"
#include "commands/increasedecreasedialog.h"
#include "commands/changestatedialog.h"
#include "commands/combospindialog.h"
#include "commands/changecolortonedialog.h"
#include "commands/combocombodialog.h"
#include "commands/weatherdialog.h"
#include "commands/picturedialog.h"
#include "commands/conditionalbranchdialog.h"
#include "commands/spinspindialog.h"
#include "commands/changemapsettingsdialog.h"
#include "commands/changeequipmentdialog.h"
#include "commands/timerdialog.h"
#include "commands/selfswitchdialog.h"
#include "commands/scrollmapdialog.h"
#include "commands/screenshakedialog.h"
#include "commands/forceactiondialog.h"
#include "commands/dealdamagedialog.h"
#include "commands/showbattleanimationdialog.h"
#include "commands/changeactornamedialog.h"
#include "commands/shopprocessingdialog.h"
#include "commands/changeactorgraphicdialog.h"
#include "commands/changepartymemberdialog.h"
#include "commands/controlswitchesdialog.h"
#include "commands/controlvariablesdialog.h"
#include "commands/choicesdialog.h"
#include "commands/transferplayerdialog.h"
#include "commands/seteventlocationdialog.h"
#include "commands/battleprocessingdialog.h"

#include "dialogs/audiodialog.h"
#include "dialogs/imagedialog.h"


EventListItem::EventListItem(QListWidget *parent, RPGMapController *mc, RPGMapInfoController *mic, QJsonObject obj) : QListWidgetItem()
{
    this->setColor(obj.value("@code").toInt());
    this->obj = obj;
    this->db = mc->getDB();
    this->mc = mc;
    this->mic = mic;
    this->parent = parent;

    this->update_text();
}

void EventListItem::setColor(int code)
{
    if ((code >= 351 && code <= 355) || code == 655) this->setForeground(grey);
    else if (code == 209 || code == 509) this->setForeground(dark_red);
    else if (code == 108 || code == 408) this->setForeground(green);
    else if ((code >= 111 && code <= 119) || code == 413 || code == 412 || code == 411) this->setForeground(blue);
    else if ((code >= 121 && code <= 129)) this->setForeground(red);
    else if ((code >= 131 && code <= 136)) this->setForeground(pink);
    else if (code >= 201 && code <= 208) this->setForeground(dark_red);
    else if (code >= 221 && code <= 225) this->setForeground(light_green);
    else if (code >= 231 && code <= 236) this->setForeground(dark_pink);
    else if (code >= 241 && code <= 251) this->setForeground(light_blue);
    else if ((code >= 301 && code <= 303) || (code >= 601 && code <= 605)) this->setForeground(orange);
    else if (code >= 311 && code <= 322) this->setForeground(cyan);
    else if (code >= 331 && code <= 340) this->setForeground(dark_pink); //TODO: should be slightly different to 231-236
    else this->setForeground(black);
}

void EventListItem::update_text()
{
    int indent_int = obj.value("@indent").toInt();
    QString indent  = "";
    for (int i = 0; i < indent_int; i++) indent += "  ";
    this->setText(indent + this->get_text(obj));
}

void EventListItem::edit_cell()
{
    int code = obj.value("@code").toInt();
    QJsonArray parameters = obj.value("@parameters").toArray();

    if (code == 101 || code == 108 || code == 355) //Multiline text
    {
        int multiline_id = code+300;
        QString full = parameters.at(0).toString() + '\n';
        int row = parent->indexFromItem(this).row();
        for (int i = 1; dynamic_cast<EventListItem*>(parent->item(row+i)) != nullptr
             && ((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == multiline_id; i++)
        {
            full += ((EventListItem*)parent->item(row+i))->get_obj().value("@parameters").toArray().at(0).toString() + '\n';
        }
        ShowTextDialog *dialog = new ShowTextDialog(nullptr, code);
        dialog->setString(full);
        dialog->show();

        QObject::connect(dialog, &ShowTextDialog::ok_clicked, [=](QString text){
            for (int i = 1; dynamic_cast<EventListItem*>(parent->item(row+1)) != nullptr
                 && ((EventListItem*)parent->item(row+1))->get_obj().value("@code").toInt() == multiline_id; i++) {
                delete parent->takeItem(row+1);
            }
            QStringList list = text.split('\n');
            if (list.last() == "")
                list.removeLast();

            if (list.length() > 0)
            {
                QJsonArray p;
                p.append(list.first());
                this->obj.insert("@parameters", p);
                this->update_text();
            }


            for (int i = 1; i < list.length(); i++)
            {
                QJsonArray parameters;
                parameters.append(list.at(i));
                QJsonObject obj_new = QJsonObject(this->obj);
                obj_new.insert("@code", multiline_id);
                obj_new.insert("@parameters", parameters);

                EventListItem *item = new EventListItem(this->parent, mc, mic,obj_new);
                parent->insertItem(row+i, item);
            }
        });
    }
    else if (code == 401 || code == 408 || code == 655 || code == 605) //Multiline text in the middle
    {
        int row = parent->indexFromItem(this).row();
        for (int i = 1; row >= 0 && dynamic_cast<EventListItem*>(parent->item(row-i)) != nullptr; i++)
        {
            if (((EventListItem*)parent->item(row-i))->get_obj().value("@code").toInt() == (code == 605 ?302 : code-300))
            {
                ((EventListItem*)parent->item(row-i))->edit_cell();
                return;
            }
        }
    }
    else if (code == 301)
    {
        //601-- win
        //602-- escape
        //603-- lose
        //604-- end
        QJsonArray p;
        p.append(parameters.at(0).toInt());
        bool can_escape = false;
        bool can_lose = false;
        int pos_602 = -1;
        int pos_603 = -1;
        int depth = 0;
        int row = parent->indexFromItem(this).row();
        for (int i = 1; dynamic_cast<EventListItem*>(parent->item(row+i)) != nullptr; i++)
        {
            if (((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 301)
            {
                depth++;
                continue;
            }
            else if (((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 604 && depth > 0)
            {
                depth--;
                continue;
            }
            else if (depth > 0)
                continue;


            if (((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 602)
            {can_escape = true; pos_602 = row+i; }
            if (((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 603)
            {can_lose = true; pos_603 = row+i; }
            if (((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 604)
                break;
        }
        p.append(can_escape);
        p.append(can_lose);


        BattleProcessingDialog *dialog = new BattleProcessingDialog(db,p);
        dialog->show();
        QObject::connect(dialog, &BattleProcessingDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();

            //both unchecked, delete all
            if (!p.at(1).toBool() && !p.at(2).toBool()
                    && row+1 < parent->count() && dynamic_cast<EventListItem*>(parent->item(row+1)) != nullptr
                    && ((EventListItem*)parent->item(row+1))->get_obj().value("@code").toInt() == 601)
            {
                while (row+1 < parent->count() && dynamic_cast<EventListItem*>(parent->item(row+1)) != nullptr
                       && ((EventListItem*)parent->item(row+1))->get_obj().value("@code").toInt() != 604)
                    delete parent->takeItem(row+1);
                delete parent->takeItem(row+1); //delete also the 604 row
            }
            //escape unchecked, delete 602
            else if (!p.at(1).toBool() && p.at(2).toBool() && pos_602 >= 0)
            {
                while (pos_602 < parent->count() && dynamic_cast<EventListItem*>(parent->item(pos_602)) != nullptr
                       && ((EventListItem*)parent->item(pos_602))->get_obj().value("@code").toInt() != 603
                       && ((EventListItem*)parent->item(pos_602))->get_obj().value("@code").toInt() != 604)
                    delete parent->takeItem(pos_602);
            }
            //can lose unchecked unchecked, delete 603
            else if (p.at(1).toBool() && !p.at(2).toBool() && pos_603 >= 0)
            {
                while (pos_603 < parent->count() && dynamic_cast<EventListItem*>(parent->item(pos_603)) != nullptr
                       && ((EventListItem*)parent->item(pos_603))->get_obj().value("@code").toInt() != 602
                       && ((EventListItem*)parent->item(pos_603))->get_obj().value("@code").toInt() != 604)
                    delete parent->takeItem(pos_603);
            }


            //if one is checked, and there is the "win" branch, create it
            if ((p.at(1).toBool() || p.at(2).toBool())
                    && row+1 < parent->count() && dynamic_cast<EventListItem*>(parent->item(row+1)) != nullptr
                    && ((EventListItem*)parent->item(row+1))->get_obj().value("@code").toInt() != 601)
            {
                QJsonObject obj_new;

                //inverse order
                obj_new = QJsonObject(this->obj);
                obj_new.insert("@code", 604);
                obj_new.insert("@parameters", QJsonArray());
                parent->insertItem(row+1,new EventListItem(parent,mc,mic, obj_new));

                obj_new = QJsonObject(this->obj);
                obj_new.insert("@code", 0);
                obj_new.insert("@indent", this->obj.value("@indent").toInt()+1);
                obj_new.insert("@parameters", QJsonArray());
                parent->insertItem(row+1,new EventListItem(parent,mc,mic, obj_new));

                obj_new = QJsonObject(this->obj);
                obj_new.insert("@code", 601);
                obj_new.insert("@parameters", QJsonArray());
                parent->insertItem(row+1,new EventListItem(parent,mc,mic, obj_new));
            }

            /*
                if (p.at(2).toBool())
                {
                    obj_new = QJsonObject(this->obj);
                    obj_new.insert("@code", 0);
                    obj_new.insert("@indent", this->obj.value("@indent").toInt()+1);
                    obj_new.insert("@parameters", QJsonArray());
                    parent->insertItem(row+1,new EventListItem(parent,mc,mic, obj_new));

                    obj_new = QJsonObject(this->obj);
                    obj_new.insert("@code", 603);
                    obj_new.insert("@parameters", QJsonArray());
                    parent->insertItem(row+1,new EventListItem(parent,mc,mic, obj_new));
                }
                if (p.at(1).toBool())
                {
                    obj_new = QJsonObject(this->obj);
                    obj_new.insert("@code", 0);
                    obj_new.insert("@indent", this->obj.value("@indent").toInt()+1);
                    obj_new.insert("@parameters", QJsonArray());
                    parent->insertItem(row+1,new EventListItem(parent,mc,mic, obj_new));

                    obj_new = QJsonObject(this->obj);
                    obj_new.insert("@code", 602);
                    obj_new.insert("@parameters", QJsonArray());
                    parent->insertItem(row+1,new EventListItem(parent,mc,mic, obj_new));
                }
                */

            //TODO
        });
    }
    else if (code == 302)
    {
        QJsonArray shop_params;
        shop_params.append(parameters);
        int row = parent->indexFromItem(this).row();
        for (int i = 1; dynamic_cast<EventListItem*>(parent->item(row+i)) != nullptr
             && ((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 605; i++)
        {
            shop_params.append(((EventListItem*)parent->item(row+i))->get_obj().value("@parameters").toArray());
        }

        ShopProcessingDialog *dialog = new ShopProcessingDialog(db,shop_params);
        dialog->show();
        QObject::connect(dialog, &ShopProcessingDialog::ok_clicked, [=](QJsonArray shop_params) {
            this->obj.insert("@parameters", shop_params.at(0).toArray());
            this->update_text();
            for (int i = 1; dynamic_cast<EventListItem*>(parent->item(row+1)) != nullptr
                 && ((EventListItem*)parent->item(row+1))->get_obj().value("@code").toInt() == 605; i++) {
                delete parent->takeItem(row+1);
            }
            for (int i = shop_params.count()-1; i > 0; i--)
            {
                QJsonObject obj_new = QJsonObject(this->obj);
                obj_new.insert("@code", 605);
                obj_new.insert("@parameters", shop_params.at(i).toArray());

                parent->insertItem(row+1,new EventListItem(parent,mc,mic, obj_new));
            }
        });
    }
    else if (code == 102) //Show Choices
    {
        //402 -- when x
        //403 -- wenn cancel
        //404 -- branch end
        int row = parent->indexFromItem(this).row();
        ChoicesDialog *dialog = new ChoicesDialog(parameters);
        dialog->show();
        QObject::connect(dialog, &ChoicesDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();

            bool has_else = (p.at(1).toInt() == 5);
            int depth = 0;
            int item = 0;
            for (int i = 1; row+i < parent->count() && dynamic_cast<EventListItem*>(parent->item(row+i)) != nullptr; i++) {
                //only for recursion
                if (((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 102)
                {
                    depth++;//depth if new show choices
                    continue;
                }
                else if (((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 404 && depth > 0)
                {
                    depth--;
                    continue;
                }
                else if (depth > 0)
                {
                    continue;
                }

                //when branch
                if (((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 402)
                {
                    if (item < p.at(0).toArray().count())
                    {
                        //change 402 to new label
                        QJsonObject o = ((EventListItem*)parent->item(row+i))->get_obj();
                        QJsonArray item_p = QJsonArray();
                        item_p.append(item);
                        item_p.append(p.at(0).toArray().at(item).toString());
                        o.insert("@parameters", item_p);
                        ((EventListItem*)parent->item(row+i))->set_obj(o);
                        ((EventListItem*)parent->item(row+i))->update_text();
                        item++;
                    }
                    else
                    {
                        //remove 402 branches
                        while (row+i < parent->count() && dynamic_cast<EventListItem*>(parent->item(row+i)) != nullptr
                               && ((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() != 403
                               && ((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() != 404)
                            delete parent->takeItem(row+i);
                        i--; //parse line again
                    }
                }
                else if (((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 403
                         || ((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 404)
                {
                    //qDebug() << item << p.at(0).toArray().count() << ((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt();
                    while (item < p.at(0).toArray().count())
                    {
                        //create more choices 402
                        QJsonObject obj1 = Factory().create_empty_event_command(); // code 0, parameters []
                        obj1.insert("@indent", obj.value("@indent").toInt()+1);

                        QJsonObject obj2 = Factory().create_empty_event_command(); // code 0, parameters []
                        obj2.insert("@indent", obj.value("@indent").toInt());
                        obj2.insert("@code", 402);
                        QJsonArray item_p = QJsonArray();
                        item_p.append(item);
                        item_p.append(p.at(0).toArray().at(item).toString());
                        obj2.insert("@parameters", item_p);
                        item++;

                        parent->insertItem(row+i, new EventListItem(parent,mc,mic,obj1)); //swapped places
                        parent->insertItem(row+i, new EventListItem(parent,mc,mic,obj2));
                        i += 2;
                    }

                    if (((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 403 && !has_else)
                    {
                        //remove "when cancel" branch
                        while (row+i < parent->count() && dynamic_cast<EventListItem*>(parent->item(row+i)) != nullptr
                               && ((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() != 404)
                            delete parent->takeItem(row+i);
                        return;
                    }
                    if (((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 404 && has_else)
                    {
                        //create 403
                        QJsonObject obj1 = Factory().create_empty_event_command(); // code 0, parameters []
                        obj1.insert("@indent", obj.value("@indent").toInt()+1);

                        QJsonObject obj2 = Factory().create_empty_event_command(); // code 0, parameters []
                        obj2.insert("@indent", obj.value("@indent").toInt());
                        obj2.insert("@code", 403);

                        parent->insertItem(row+i, new EventListItem(parent,mc,mic,obj1)); //swapped places
                        parent->insertItem(row+i, new EventListItem(parent,mc,mic,obj2));
                        return;
                    }

                    return; //when reaching 403 or 404 and else branch handling is finished, we can stop
                }
            }

        });
    }
    else if (code == 104)
    {
        ChangeTextOptionsDialog *dialog = new ChangeTextOptionsDialog;
        dialog->setParameters(parameters);
        dialog->show();
        QObject::connect(dialog, &ChangeTextOptionsDialog::ok_clicked, [=](QJsonArray parameters) {
            this->obj.insert("@parameters", parameters);
            this->update_text();
        });
    }
    else if (code == 106 || code == 235 || code == 242 || code == 246)
    {
        QString text1;
        QString text2;
        switch (code)
        {
        case 106: text1 = "Wait"; text2 = "Frames:"; break;
        case 235: text1 = "Erase Picture"; text2 = "Pic. No.:"; break;
        case 242: text1 = "Fade Out BGM"; text2 = "Time in sec:"; break;
        case 246: text1 = "Fade Out BGS"; text2 = "Time in sec:"; break;
        }

        bool ok;
        int v = QInputDialog::getInt(this->parent, text1, text2, parameters.at(0).toInt(),1,999, 1, &ok);
        if (ok){
            QJsonArray newparams;
            newparams.append(v);
            this->obj.insert("@parameters", newparams);
        }
    }
    else if (code == 118 || code == 119)
    {
        bool ok;
        QString label = QInputDialog::getText(this->parent, (code == 119 ? QString("Jump to ") : "") + "Label", (code == 119 ? QString("Jump to ") : "") + "Label:", QLineEdit::Normal, parameters.at(0).toString(), &ok);
        if (ok){
            QJsonArray newparams;
            newparams.append(label);
            this->obj.insert("@parameters", newparams);
        }
    }
    else if (code == 134 || code == 135 || code == 136 || code == 208) //2 radio buttons
    {
        RadioDialog *dialog = new RadioDialog(nullptr, code, parameters.at(0).toInt() == 0);
        dialog->show();
        QObject::connect(dialog, &RadioDialog::ok_clicked, [=](QJsonArray array){ this->obj.insert("@parameters", array); this->update_text(); });
    }
    else if (code == 132 || code == 133 || code == 241 || code == 245 || code == 249 || code == 250) //Audio Dialog
    {
        int type;
        if (code == 132 || code == 241) type = AudioDialog::BGM;
        else if (code == 245) type = AudioDialog::BGS;
        else if (code == 133 || code == 249) type = AudioDialog::ME;
        else //if (code == 250)
            type = AudioDialog::SE;
        AudioDialog *dialog = new AudioDialog(this->db, parameters.at(0).toObject().value("@name").toString(), parameters.at(0).toObject().value("@volume").toInt(), parameters.at(0).toObject().value("@pitch").toInt(), type);
        dialog->show();
        QObject::connect(dialog, &AudioDialog::ok_clicked, [=](QString name, int volume, int pitch){
            QJsonArray param;
            param.append(Factory().create_audiofile(name, volume,pitch));
            this->obj.insert("@parameters", param);
            this->update_text();
        });

    }
    else if (code == 105 || code == 117 || code == 314 || code == 334 || code == 335) //single combo box
    {
        SingleComboDialog *dialog = new SingleComboDialog(this->db, code, parameters.at(0).toInt());
        dialog->show();
        QObject::connect(dialog, &SingleComboDialog::ok_clicked, [=](QJsonArray parameters) { this->obj.insert("@parameters", parameters); this->update_text(); });
    }
    else if ((code >= 125 && code <= 128) || code == 311 || code == 312 || (code >= 315 && code <= 317) || code == 331 || code == 332) //single combo box
    {
        //qDebug() << parameters;
        IncreaseDecreaseDialog *dialog = new IncreaseDecreaseDialog(this->db, code, parameters);
        dialog->show();
        QObject::connect(dialog, &IncreaseDecreaseDialog::ok_clicked, [=](QJsonArray parameters) {
            //qDebug() << parameters;
            this->obj.insert("@parameters", parameters); this->update_text(); });
    }
    else if (code == 131 || code == 222)
    {
        ImageDialog *dialog = new ImageDialog(db, (code == 131 ? ImageDialog::WINDOWSKINS : ImageDialog::TRANSITIONS), parameters.at(0).toString());
        dialog->show();
        QObject::connect(dialog, &ImageDialog::ok_clicked, [=](QString img) {
            QJsonArray p;
            p.append(img);
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 313 || code == 333 || code == 318) //Change (Enemy) State and Skills
    {
        ChangeStateDialog *dialog = new ChangeStateDialog(db, code, parameters);
        dialog->show();
        QObject::connect(dialog, &ChangeStateDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 103 || code == 303) //Input Number Processing, Name Input Processing
    {
        ComboSpinDialog *dialog = new ComboSpinDialog(db, code, parameters);
        dialog->show();
        QObject::connect(dialog, &ComboSpinDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 205 || code == 223 || code == 234 || code == 224) //Color Tones
    {
        ChangeColorToneDialog *dialog = new ChangeColorToneDialog(db, code, parameters);
        dialog->show();
        QObject::connect(dialog, &ChangeColorToneDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 207 || code == 321 || code == 336) //Show Animation, Actor Class, Enemy Transform
    {
        ComboComboDialog *dialog = new ComboComboDialog(db, mc, code, parameters);
        dialog->show();
        QObject::connect(dialog, &ComboComboDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 236) //weather
    {
        WeatherDialog *dialog = new WeatherDialog(code, parameters);
        dialog->show();
        QObject::connect(dialog, &WeatherDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 231 || code == 232) //Show/Move Picture
    {
        PictureDialog *dialog = new PictureDialog(db,code, parameters);
        dialog->show();
        QObject::connect(dialog, &PictureDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 206 || code == 233) //Change Fog Opacity / Rotate Picture
    {
        SpinSpinDialog *dialog = new SpinSpinDialog(code, parameters);
        dialog->show();
        QObject::connect(dialog, &SpinSpinDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 204) //Change Map Settings
    {
        ChangeMapSettingsDialog *dialog = new ChangeMapSettingsDialog(db, parameters);
        dialog->show();
        QObject::connect(dialog, &ChangeMapSettingsDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 319) //Change Equipment
    {
        ChangeEquipmentDialog *dialog = new ChangeEquipmentDialog(db, parameters);
        dialog->show();
        QObject::connect(dialog, &ChangeEquipmentDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 121) //Control Switches
    {
        ControlSwitchesDialog *dialog = new ControlSwitchesDialog(db,parameters);
        dialog->show();
        QObject::connect(dialog, &ControlSwitchesDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 122) //Control Variables
    {
        ControlVariablesDialog *dialog = new ControlVariablesDialog(db,mc,parameters);
        dialog->show();
        QObject::connect(dialog, &ControlVariablesDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 123) //Self Switch
    {
        SelfSwitchDialog *dialog = new SelfSwitchDialog(parameters);
        dialog->show();
        QObject::connect(dialog, &SelfSwitchDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 124) //Timer
    {
        TimerDialog *dialog = new TimerDialog(parameters);
        dialog->show();
        QObject::connect(dialog, &TimerDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 129) //Change Party Member
    {
        ChangePartyMemberDialog *dialog = new ChangePartyMemberDialog(db,parameters);
        dialog->show();
        QObject::connect(dialog, &ChangePartyMemberDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 201) //Transfer Player
    {
        TransferPlayerDialog *dialog = new TransferPlayerDialog(db,parameters);
        dialog->show();
        QObject::connect(dialog, &TransferPlayerDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 202) //Set Event Location
    {
        SetEventLocationDialog *dialog = new SetEventLocationDialog(db,mc,parameters);
        dialog->show();
        QObject::connect(dialog, &SetEventLocationDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 203) //Scroll Map
    {
        ScrollMapDialog *dialog = new ScrollMapDialog(parameters);
        dialog->show();
        QObject::connect(dialog, &ScrollMapDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 225) //Screen Shake
    {
        ScreenShakeDialog *dialog = new ScreenShakeDialog(parameters);
        dialog->show();
        QObject::connect(dialog, &ScreenShakeDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 320) //Change Actor Name
    {
        ChangeActorNameDialog *dialog = new ChangeActorNameDialog(db, parameters);
        dialog->show();
        QObject::connect(dialog, &ChangeActorNameDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 322) //Change Actor Graphic
    {
        ChangeActorGraphicDialog *dialog = new ChangeActorGraphicDialog(db, parameters);
        dialog->show();
        QObject::connect(dialog, &ChangeActorGraphicDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 337) //Show Battle Animation
    {
        ShowBattleAnimationDialog *dialog = new ShowBattleAnimationDialog(db, parameters);
        dialog->show();
        QObject::connect(dialog, &ShowBattleAnimationDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 338) //Deal Damage
    {
        DealDamageDialog *dialog = new DealDamageDialog(db, parameters);
        dialog->show();
        QObject::connect(dialog, &DealDamageDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 339) //Force Action
    {
        ForceActionDialog *dialog = new ForceActionDialog(db, parameters);
        dialog->show();
        QObject::connect(dialog, &ForceActionDialog::ok_clicked, [=](QJsonArray p) {
            this->obj.insert("@parameters", p);
            this->update_text();
        });
    }
    else if (code == 111) // Conditional Branch
    {
        bool else_branch = false;
        int depth = 0;
        int row = parent->indexFromItem(this).row();
        for (int i = 1; row+i < parent->count() && dynamic_cast<EventListItem*>(parent->item(row+i)) != nullptr; i++)
        {
            if (((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 111)
                depth++;
            else if (((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 412 && depth > 0)
                depth--;
            else if (((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 412 && depth == 0)
            {
                else_branch = false;
                break;
            }
            else if (((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 411 && depth == 0)
            {
                else_branch = true;
                break;
            }
        }

        ConditionalBranchDialog *dialog = new ConditionalBranchDialog(db, mc, parameters, else_branch);
        dialog->show();
        QObject::connect(dialog, &ConditionalBranchDialog::ok_clicked, [=](QJsonArray p, bool else_branch) {
            this->obj.insert("@parameters", p);
            this->update_text();


            int depth = 0;
            int row = parent->indexFromItem(this).row();
            for (int i = 1; row+i < parent->count() && dynamic_cast<EventListItem*>(parent->item(row+i)) != nullptr; i++)
            {
                if (((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 111)
                    depth++;
                else if (((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 412 && depth > 0)
                    depth--;
                else if (((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 412 && depth == 0 && !else_branch)
                    break; //good
                else if (((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 411 && depth == 0 && else_branch)
                    break; //good
                else if (((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 412 && depth == 0 && else_branch)
                {
                    //not good, create branch
                    parent->insertItem(row+i, new EventListItem(this->parent, mc, mic, Factory().create_event_command(0,obj.value("@indent").toInt()+1, QJsonArray())));
                    parent->insertItem(row+i, new EventListItem(this->parent, mc, mic, Factory().create_event_command(411,obj.value("@indent").toInt(), QJsonArray())));
                    break;
                }
                else if (((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() == 411 && depth == 0 && !else_branch)
                {
                    //not good, delete branch
                    while (parent->count() > row+i && ((EventListItem*)parent->item(row+i))->get_obj().value("@code").toInt() != 412)
                        delete parent->takeItem(row+i);
                    break;
                }


            }
        });


    }
    else
        qDebug() << this->obj;

    this->update_text();
}

QString EventListItem::get_text(QJsonObject obj)
{
    QString text;
    int code = obj.value("@code").toInt();
    QJsonArray parameters = obj.value("@parameters").toArray();

    if (code == 0)
        text = "@>";
    else if (code == 101)
        text = "@>Text: " + parameters.at(0).toString();
    else if (code == 401)
        text = " :      "  + parameters.at(0).toString();
    else if (code == 102)
    {
        QJsonArray choices = parameters.at(0).toArray();
        //int std = parameters.at(1).toInt();
        text = "@>Show Choices: ";
        for (int i = 0; i < choices.count(); i++)
            text += choices.at(i).toString() + (i < choices.count()-1 ? ", " : "");
    }
    else if (code == 402)
        text = " : When " + QString("[%1]").arg(parameters.at(1).toString());
    else if (code == 403)
        text = " : When Cancel";
    else if (code == 404 || code == 412) //404 for choices, 412 for conditional branch
        text = " : Branch End";
    else if (code == 103)
    {
        text ="@>Input Number: " + QString("[%1: %2], %3 digit(s)")
                .arg(parameters.at(0).toInt(), 4, 10, QChar('0'))
                .arg(db->get_variable_name(parameters.at(0).toInt()))
                .arg(parameters.at(1).toInt());
    }
    else if (code == 104)
        text = "@>Change Text Options: " + this->text_option_pos.at(parameters.at(0).toInt()) + ", " + this->text_option_win.at(parameters.at(1).toInt());
    else if (code == 105)
        text = "@>Button Input Processing: " + QString("[%1: %2]")
                .arg(parameters.at(0).toInt(), 4, 10, QChar('0'))
                .arg(this->db->get_variable_name(parameters.at(0).toInt()));
    else if (code == 106)
        text = "@>Wait: " + QString("%1 frame(s)").arg(parameters.at(0).toInt());
    else if (code == 108)
        text = "@>Comment: " + parameters.at(0).toString();
    else if (code == 408)
        text = " :       : " + parameters.at(0).toString();
    else if (code == 111)
    {
        //Conditional Branch
        text = "@>Conditional Branch: ";
        switch (parameters.at(0).toInt())
        {
        case 0: text += QString("Switch [%1: %2] == %3")
                    .arg(parameters.at(1).toInt(),4,10, QChar('0'))
                    .arg(db->get_switch_name(parameters.at(1).toInt()))
                    .arg(this->text_on_off.at(parameters.at(2).toInt())); break; // switch
        case 1: text += QString("Variable [%1: %2] %3 %4")
                    .arg(parameters.at(1).toInt(),4,10, QChar('0'))
                    .arg(db->get_variable_name(parameters.at(1).toInt()))
                    .arg(this->text_compare_op.at(parameters.at(4).toInt()))
                    .arg(parameters.at(2).toInt()==0 ?
                             QString::number(parameters.at(3).toInt())
                           : QString("Variable [%1: %2]").arg(parameters.at(3).toInt(),4,10, QChar('0')).arg(db->get_variable_name(parameters.at(3).toInt()))); break; // variable
        case 2: text += QString("Self Switch %1 == %2")
                    .arg(parameters.at(1).toString())
                    .arg(this->text_on_off.at((parameters.at(2).toInt()))); break; // self switch
        case 3: text += QString("Timer %1 min %2 sec %3")
                    .arg(parameters.at(1).toInt()/60)
                    .arg(parameters.at(1).toInt()%60)
                    .arg(parameters.at(2).toInt() == 1 ? "or less" : "or more"); break; // timer
        case 4:
            text += QString("[%1] is ").arg(db->get_object_name(RPGDB::ACTORS, parameters.at(1).toInt()));
            if (parameters.at(2).toInt() == 0)
                text += "in the party";
            else if (parameters.at(2).toInt() == 1)
                text += QString("name '%1' applied").arg(parameters.at(3).toString());
            else if (parameters.at(2).toInt() == 2)
                text += QString("[%1] learned").arg(db->get_object_name(RPGDB::SKILLS, parameters.at(3).toInt()));
            else if (parameters.at(2).toInt() == 3)
                text += QString("[%1] equipped").arg(db->get_object_name(RPGDB::WEAPONS, parameters.at(3).toInt()));
            else if (parameters.at(2).toInt() == 4)
                text += QString("[%1] equipped").arg(db->get_object_name(RPGDB::ARMORS, parameters.at(3).toInt()));
            else if (parameters.at(2).toInt() == 5)
                text += QString("[%1] inflicted").arg(db->get_object_name(RPGDB::STATES, parameters.at(3).toInt()));
            break; // actor
        case 5: text += QString("[%1.] is %2")
                    .arg(parameters.at(1).toInt()+1)
                    .arg(parameters.at(2).toInt() == 0 ?
                             "appeared" : QString("[%1] inflicted").arg(db->get_object_name(RPGDB::STATES,parameters.at(3).toInt()))); break; // enemy
        case 6: text += QString("%1 is facing %2")
                    .arg(parameters.at(1).toInt() == -1 ? "Player" :
                                                          parameters.at(1).toInt() == 0 ? "This event" : QString("[%1]").arg(
                                                                                              mc->current_map()->object().value("@events").toObject().value(QString::number(parameters.at(1).toInt())).toObject().value("@name").toString()))
                    .arg(this->text_directions.at(parameters.at(2).toInt()/2-1));
                    break; // character
        case 7: text += QString("Gold %2 %3").arg(parameters.at(1).toInt())
                    .arg(parameters.at(2).toInt() == 0 ? "or more" : "or less"); break; // gold
        case 8: text += QString("[%1] in inventory").arg(db->get_object_name(RPGDB::ITEMS, parameters.at(1).toInt())); break; // item
        case 9: text += QString("[%1] in inventory").arg(db->get_object_name(RPGDB::WEAPONS, parameters.at(1).toInt())); break; // weapon
        case 10: text += QString("[%1] in inventory").arg(db->get_object_name(RPGDB::ARMORS, parameters.at(1).toInt())); break; // armor
        case 11: text += QString("The %1 button is being pressed").arg(parameters.at(1).toInt() <= 8 ?
                                                                           this->text_directions.at(parameters.at(1).toInt()/2-1)
                                                                         : this->text_buttons.at(parameters.at(1).toInt()-11)); break; // button
        case 12: text += QString("Script: %1").arg(parameters.at(1).toString());break; // script
        }
    }
    else if (code == 411)
        text = " : Else";
    else if (code == 112)
        text = "@>Loop";
    else if (code == 113)
        text = "@>Break Loop";
    else if (code == 413)
        text = " : Repeat Above";
    else if (code == 115)
        text = "@>Exit Event Processing";
    else if (code == 116)
        text = "@>Erase Event";
    else if (code == 117)
        text = "@>Call Common Event: " + db->get_object_name(RPGDB::COMMONEVENTS,parameters.at(0).toInt());
    else if (code == 118)
        text = "@>Label: " + parameters.at(0).toString();
    else if (code == 119)
        text = "@>Jump to Label: " + parameters.at(0).toString();
    else if (code == 121)
    {
        text = "@>Control Switches: " +
                ((parameters.at(0) == parameters.at(1)) ? QString("[%1: %2] = %3")
                                                         .arg(parameters.at(0).toInt(),4,10,QChar('0'))
                                                         .arg(db->get_switch_name(parameters.at(0).toInt()))
                                                         .arg(this->text_on_off.at(parameters.at(2).toInt()))
                                                       : QString("[%1..%2] = %3")
                                                         .arg(parameters.at(0).toInt(),4,10,QChar('0'))
                                                         .arg(parameters.at(1).toInt(),4,10,QChar('0'))
                                                         .arg(this->text_on_off.at(parameters.at(2).toInt())));
    }
    else if (code == 122)
    {
        text = "@>Control Variables: " +
                ((parameters.at(0).toInt() == parameters.at(1).toInt()) ? QString("[%1: %2] %3 ")
                                                         .arg(parameters.at(0).toInt(),4,10,QChar('0'))
                                                         .arg(db->get_variable_name(parameters.at(0).toInt()))
                                                         .arg(this->text_var_operators.at(parameters.at(2).toInt()))
                                                       : QString("[%1..%2] %3 ")
                                                         .arg(parameters.at(0).toInt(),4,10,QChar('0'))
                                                         .arg(parameters.at(1).toInt(),4,10,QChar('0'))
                                                         .arg(this->text_var_operators.at(parameters.at(2).toInt())));
        switch(parameters.at(3).toInt())
        {
        case 0: text += QString::number(parameters.at(4).toInt()); break;
        case 1: text += QString("Variable [%1: %2]").arg(parameters.at(4).toInt(),4,10,QChar('0')).arg(db->get_variable_name(parameters.at(4).toInt())); break;
        case 2: text += QString("Random No (%1..%2)").arg(parameters.at(4).toInt()).arg(parameters.at(5).toInt()); break;
        case 3: text += QString("[%1] In Inventory").arg(db->get_object_name(RPGDB::ITEMS, parameters.at(4).toInt())); break;
        case 4: text += QString("[%1]'s %2").arg(db->get_object_name(RPGDB::ACTORS, parameters.at(4).toInt())).arg(this->text_stat_vars.at(parameters.at(5).toInt())); break;
        case 5: text += QString("[%1.]'s %2").arg(parameters.at(4).toInt()+1).arg(this->text_stat_vars.at(parameters.at(5).toInt()+2)); break; //level,exp only for actors
        case 6:

            text += QString("%1's %2")
                    .arg( (parameters.at(4).toInt() == -1 ? "Player" : (
                                                                (parameters.at(4).toInt() == 0 ? "This event" :
                                                                                                "[" + mc->current_map()->object().value("@events").toObject().value(QString::number(parameters.at(4).toInt())).toObject().value("@name").toString() + "]") )  ))
                    .arg(this->text_map_vars.at(parameters.at(5).toInt())); break;
        case 7: text += this->text_other_vars.at(parameters.at(4).toInt());  break;
        }
    }
    else if (code == 123)
        text += "@>Control Self Switch: " + QString("%1 = %2").arg(parameters.at(0).toString()).arg(this->text_on_off.at(parameters.at(1).toInt()));
    else if (code == 124)
        text += "@>Control Timer: " + (parameters.at(0).toInt() == 1 ? "Stop" : QString("Startup (%1 min. %2 sec.)").arg(parameters.at(1).toInt()/60).arg(parameters.at(1).toInt()%60));
    else if (code == 125)
    {
        text+= "@>Change Gold: ";
        if (parameters.at(1).toInt() == 0)
            text += this->text_pm.at(parameters.at(0).toInt()) + " " + QString::number(parameters.at(2).toInt());
        else
            text += QString("%1 Variable [%2: %3]").arg(this->text_pm.at(parameters.at(0).toInt())).arg(parameters.at(2).toInt(),4,10,QChar('0')).arg(db->get_variable_name(parameters.at(2).toInt()));
    }
    else if (code == 126)
        text += "@>Change Items: " + QString("[%1], %2 %3")
                .arg(db->get_object_name(RPGDB::ITEMS, parameters.at(0).toInt()))
                .arg(this->text_pm.at(parameters.at(1).toInt()))
                .arg( parameters.at(2).toInt()  == 0 ? QString::number(parameters.at(3).toInt()) : QString("Variable [%1:%2]")
                                                       .arg(parameters.at(3).toInt(),4,10,QChar('0'))
                                                       .arg(db->get_variable_name(parameters.at(3).toInt())));
    else if (code == 127)
        text += "@>Change Weapons: " + QString("[%1], %2 %3")
                .arg(db->get_object_name(RPGDB::WEAPONS, parameters.at(0).toInt()))
                .arg(this->text_pm.at(parameters.at(1).toInt()))
                .arg( parameters.at(2).toInt()  == 0 ? QString::number(parameters.at(3).toInt()) : QString("Variable [%1:%2]")
                                                       .arg(parameters.at(3).toInt(),4,10,QChar('0'))
                                                       .arg(db->get_variable_name(parameters.at(3).toInt())));
    else if (code == 128)
        text += "@>Change Armor: " + QString("[%1], %2 %3")
                .arg(db->get_object_name(RPGDB::ARMORS, parameters.at(0).toInt()))
                .arg(this->text_pm.at(parameters.at(1).toInt()))
                .arg( parameters.at(2).toInt()  == 0 ? QString::number(parameters.at(3).toInt()) : QString("Variable [%1:%2]")
                                                       .arg(parameters.at(3).toInt(),4,10,QChar('0'))
                                                       .arg(db->get_variable_name(parameters.at(3).toInt())));
    else if (code == 129)
        text += "@>Change Party Member: "
                + (parameters.at(1).toInt() == 0 ? QString("Add [%1]").arg(db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()))
                                                 : QString("Remove [%1]").arg(db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt())))
                + (parameters.at(1).toInt() == 0 && parameters.at(2).toInt() == 1 ? ", Initialize": "");
    else if (code == 131)
        text += "@>Change Windowskin: " + QString("'%1'").arg(parameters.at(0).toString());
    else if (code == 132)
        text += "@>Change Battle BGM: " + QString("'%1', %2, %3").arg(parameters.at(0).toObject().value("@name").toString()).arg(parameters.at(0).toObject().value("@volume").toInt()).arg(parameters.at(0).toObject().value("@pitch").toInt());
    else if (code == 133)
        text += "@>Change Battle End ME: " + QString("'%1', %2, %3").arg(parameters.at(0).toObject().value("@name").toString()).arg(parameters.at(0).toObject().value("@volume").toInt()).arg(parameters.at(0).toObject().value("@pitch").toInt());
    else if (code == 134)
        text += "@>Change Save Access: " + this->text_en_dis.at(parameters.at(0).toInt());
    else if (code == 135)
        text += "@>Change Menu Access: " + this->text_en_dis.at(parameters.at(0).toInt());
    else if (code == 136)
        text += "@>Change Encounter: " + this->text_en_dis.at(parameters.at(0).toInt());
    else if (code == 201)
        text += "@>Transfer Player: " + (parameters.at(0).toInt() == 0 ?
                                             QString("[%1: %2], (%3,%4)")
                                             .arg(parameters.at(1).toInt(),3,10,QChar('0'))
                                             .arg(this->mic->get_name(parameters.at(1).toInt()))
                                             .arg(parameters.at(2).toInt(),3,10,QChar('0'))
                                             .arg(parameters.at(3).toInt(),3,10,QChar('0'))
                                           : QString("Variable [%1][%2][%3]")
                                             .arg(parameters.at(1).toInt(),4,10,QChar('0'))
                                             .arg(parameters.at(2).toInt(),4,10,QChar('0'))
                                             .arg(parameters.at(3).toInt(),4,10,QChar('0')))
                                             + (parameters.at(4).toInt() == 0 ? "" : ", " + this->text_directions.at(parameters.at(4).toInt()/2-1))
                                             + (parameters.at(5).toInt() == 1 ? ", No Fade" : "");
    else if (code == 202)
        text += "@>Set Event Location: " + (parameters.at(0).toInt() == 0 ?
                                                "This event, " :
                                                "[" + mc->current_map()->object().value("@events").toObject().value(QString::number(parameters.at(0).toInt())).toObject().value("@name").toString() +"], ")
                + (parameters.at(1).toInt() == 0 ? QString("(%1,%2)").arg(parameters.at(2).toInt(),3,10,QChar('0')).arg(parameters.at(3).toInt(),3,10,QChar('0')) :
                  (parameters.at(1).toInt() == 1 ? QString("Variable [%1:%2]").arg(parameters.at(2).toInt(),4,10,QChar('0')).arg(parameters.at(3).toInt(),4,10,QChar('0')) :
                                                   QString("Switch with [%1]").arg(mc->current_map()->object().value("@events").toObject().value(QString::number(parameters.at(2).toInt())).toObject().value("@name").toString())))
                + (parameters.at(4).toInt() != 0 ? ", " +this->text_directions.at(parameters.at(4).toInt()/2-1) : "");
    else if (code == 203)
        text += "@>Scroll Map: " + this->text_directions.at(parameters.at(0).toInt()/2-1) + ", " + QString::number(parameters.at(1).toInt()) + ", " + QString::number(parameters.at(2).toInt());
    else if (code == 204)
        text += "@>Change Map Settings: " + (parameters.at(0).toInt() == 0 ?  QString("Panorama = '%1', %2").arg(parameters.at(1).toString()).arg(parameters.at(2).toInt()) :
                                            parameters.at(0).toInt() == 1 ? QString("Fog = '%1', %2, %3, %4, %5, %6, %7")
                                                                          .arg(parameters.at(1).toString())
                                                                          .arg(parameters.at(2).toInt())
                                                                          .arg(parameters.at(3).toInt())
                                                                          .arg(this->text_blend.at(parameters.at(4).toInt()))
                                                                            .arg(parameters.at(5).toInt())
                                                                            .arg(parameters.at(6).toInt())
                                                                            .arg(parameters.at(7).toInt())
                                                                          :
                                                                            QString("Battleback = '%1'").arg(parameters.at(1).toString()));
    else if (code == 205)
        text += "@>Change Fog Color Tone: " + QString("(%1,%2,%3,%4), @%5")
                .arg(parameters.at(0).toObject().value("r").toInt())
                .arg(parameters.at(0).toObject().value("g").toInt())
                .arg(parameters.at(0).toObject().value("b").toInt())
                .arg(parameters.at(0).toObject().value("alpha_gray").toInt())
                .arg(parameters.at(1).toInt());
    else if (code == 206)
        text += "@>Change Fog Opacity: " + QString("%1, @%2")
                .arg(parameters.at(0).toInt())
                .arg(parameters.at(1).toInt());
    else if (code == 207)
        text += "@>Show Animation: " + (parameters.at(0).toInt() == -1 ? "Player"
                                                                       : (parameters.at(0).toInt() == 0 ? "This event" : "[" + QString(mc->current_map()->object().value("@events").toObject().value(QString::number(parameters.at(0).toInt())).toObject().value("@name").toString() +"]")))
                + ", " + QString("[%1]").arg(db->get_object_name(RPGDB::ANIMATIONS,parameters.at(1).toInt()));
    else if (code == 208)
        text += "@>Change Transparent Flag: " + QString(parameters.at(0).toInt() == 0 ? "Transparency" : "Normal");
    else if (code == 209)
    {
        text += "@>Set Move Route: " + QString(parameters.at(0).toInt() == -1 ? "Player" :
                                       (parameters.at(0).toInt() == 0 ? "This event" :
                                                                        "[" +mc->current_map()->object().value("@events").toObject().value(QString::number(parameters.at(0).toInt())).toObject().value("@name").toString() + "]"));
        if (parameters.at(1).toObject().value("@repeat").toBool() || parameters.at(1).toObject().value("@repeat").toBool() )
        {
            text += " (";
            if (parameters.at(1).toObject().value("@repeat").toBool())
                text += "Repeat Action";
            if (parameters.at(1).toObject().value("@repeat").toBool() && parameters.at(1).toObject().value("@skippable").toBool() )
                text += ", ";
            if (parameters.at(1).toObject().value("@skippable").toBool())
                text += "Ignore If Can't Move";

            text += ")";
        }
    }
    else if (code == 509)
    {
        int code = parameters.at(0).toObject().value("@code").toInt();
        QJsonArray mr_params = parameters.at(0).toObject().value("@parameters").toArray();
        if (code >= 1 && code <= 45)
        {
                text += " :              : $>" + this->text_move_routes.at(code-1);
                if (code == 14)
                    text += QString("%1, %2").arg(mr_params.at(0).toInt()).arg(mr_params.at(1).toInt());
                else if (code == 15)
                    text += QString("%1 frame(s)").arg(mr_params.at(0).toInt());
                else if (code == 27 || code == 28)
                    text += QString("%1").arg(mr_params.at(0).toInt(),4,10, QChar('0'));
                else if (code == 29 || code == 30 || code == 42)
                    text += QString("%1").arg(mr_params.at(0).toInt());
                else if (code == 41)
                    text += QString("'%1', %2, %3, %4").arg(mr_params.at(0).toString()).arg(mr_params.at(1).toInt()).arg(mr_params.at(2).toInt()).arg(mr_params.at(3).toInt());
                else if (code == 43)
                    text += this->text_blend.at(mr_params.at(0).toInt());
                else if (code == 44)
                    text += QString("'%1', %2, %3").arg(mr_params.at(0).toObject().value("@name").toString()).arg(mr_params.at(0).toObject().value("@volume").toInt()).arg(mr_params.at(0).toObject().value("@pitch").toInt());
                else if (code == 45)
                    text += mr_params.at(0).toString();
        }
    }
    else if (code == 210)
        text += "@>Wait for Move's Completition";
    else if (code == 221)
        text += "@>Prepare for Transistion";
    else if (code == 222)
        text += "@>Execute Transistion: '" + parameters.at(0).toString() + "'";
    else if (code == 223)
        text += "@>Change Screen Color Tone: " + QString("(%1,%2,%3,%4), @%5")
                .arg(parameters.at(0).toObject().value("r").toInt())
                .arg(parameters.at(0).toObject().value("g").toInt())
                .arg(parameters.at(0).toObject().value("b").toInt())
                .arg(parameters.at(0).toObject().value("alpha_gray").toInt())
                .arg(parameters.at(1).toInt());
    else if (code == 224)
        text += "@>Screen Flash: " + QString("(%1, %2, %3, %4), @%5")
                .arg(parameters.at(0).toObject().value("r").toInt())
                .arg(parameters.at(0).toObject().value("g").toInt())
                .arg(parameters.at(0).toObject().value("b").toInt())
                .arg(parameters.at(0).toObject().value("alpha_gray").toInt())
                .arg(parameters.at(1).toInt());
    else if (code == 225)
        text += "@>Screen Shake: " + QString("%1, %2, @%3")
                .arg(parameters.at(0).toInt())
                .arg(parameters.at(1).toInt())
                .arg(parameters.at(2).toInt());
    else if (code == 231)
        text += "@>Show Picture: " + QString::number(parameters.at(0).toInt())
                + ", '" + parameters.at(1).toString() + "', "
                + (parameters.at(2).toInt() == 0 ? "Upper Left " : "Center ")
                + (parameters.at(3).toInt() == 0 ? QString("(%1, %2), ").arg(parameters.at(4).toInt()).arg(parameters.at(5).toInt())
                                                 : QString("(Variable [%1][%2]), ").arg(parameters.at(4).toInt(),4,10,QChar('0')).arg(parameters.at(5).toInt(),4,10,QChar('0')) )
                + QString("(%1%, %2%)").arg(parameters.at(6).toInt()).arg(parameters.at(7).toInt())
                + ", " + QString::number(parameters.at(8).toInt()) + ", "
                + this->text_blend.at(parameters.at(9).toInt());
    else if (code == 232)
        text += "@>Move Picture: " + QString::number(parameters.at(0).toInt())
                + ", @" + QString::number(parameters.at(1).toInt()) + ", " +
                + (parameters.at(2).toInt() == 0 ? "Upper Left " : "Center ")
                + (parameters.at(3).toInt() == 0 ? QString("(%1, %2), ").arg(parameters.at(4).toInt()).arg(parameters.at(5).toInt())
                                                 : QString("(Variable [%1][%2]), ").arg(parameters.at(4).toInt(),4,10,QChar('0')).arg(parameters.at(5).toInt(),4,10,QChar('0')) )
                + QString("(%1%, %2%)").arg(parameters.at(6).toInt()).arg(parameters.at(7).toInt())
                + ", " + QString::number(parameters.at(8).toInt()) + ", "
                + this->text_blend.at(parameters.at(9).toInt());
    else if (code == 233)
        text += "@>Rotate Picture: " + QString::number(parameters.at(0).toInt()) + ", " + QString::number(parameters.at(1).toInt());
    else if (code == 234)
        text += "@>Change Picture Color Tone: " + QString("%1, (%2,%3,%4,%5), @%6")
                .arg(parameters.at(0).toInt())
                .arg(parameters.at(1).toObject().value("r").toInt())
                .arg(parameters.at(1).toObject().value("g").toInt())
                .arg(parameters.at(1).toObject().value("b").toInt())
                .arg(parameters.at(1).toObject().value("alpha_gray").toInt())
                .arg(parameters.at(2).toInt());
    else if (code == 235)
        text += "@>Erase Picture: " + QString::number(parameters.at(0).toInt());
    else if (code == 236)
        text += "@>Set Weather Effects: " + this->text_weather.at(parameters.at(0).toInt()) + ", "
                + (parameters.at(0).toInt() != 0 ? QString::number(parameters.at(1).toInt()) + ", " : "")
                + QString("@%1").arg(parameters.at(2).toInt());
    else if (code == 241)
        text += "@>Play BGM: " + QString("'%1', %2, %3").arg(parameters.at(0).toObject().value("@name").toString()).arg(parameters.at(0).toObject().value("@volume").toInt()).arg(parameters.at(0).toObject().value("@pitch").toInt());
    else if (code == 242)
        text += "@>Fade out BGM: " + QString::number(parameters.at(0).toInt()) + " sec.";
    else if (code == 245)
        text += "@>Play BGS: " + QString("'%1', %2, %3").arg(parameters.at(0).toObject().value("@name").toString()).arg(parameters.at(0).toObject().value("@volume").toInt()).arg(parameters.at(0).toObject().value("@pitch").toInt());
    else if (code == 246)
        text += "@>Fade out BGS: " + QString::number(parameters.at(0).toInt()) + " sec.";
    else if (code == 247)
        text += "@>Memorize BGM/BGS";
    else if (code == 248)
        text += "@>Restore BGM/BGS";
    else if (code == 249)
        text += "@>Play ME: " + QString("'%1', %2, %3").arg(parameters.at(0).toObject().value("@name").toString()).arg(parameters.at(0).toObject().value("@volume").toInt()).arg(parameters.at(0).toObject().value("@pitch").toInt());
    else if (code == 250)
        text += "@>Play SE: " + QString("'%1', %2, %3").arg(parameters.at(0).toObject().value("@name").toString()).arg(parameters.at(0).toObject().value("@volume").toInt()).arg(parameters.at(0).toObject().value("@pitch").toInt());
    else if (code == 251)
        text += "@>Stop SE";
    else if (code == 301)
        text += "@>Battle Processing: " + db->get_object_name(RPGDB::TROOPS, parameters.at(0).toInt());
    else if (code == 601)
        text += ": If Win";
    else if (code == 602)
        text += ": If Escape";
    else if (code == 603)
        text += ": If Lose";
    else if (code == 604)
        text += ": Branch End";
    else if (code == 302)
        text += "@>Shop Processing: [" + (parameters.at(0).toInt() == 0 ? db->get_object_name(RPGDB::ITEMS, parameters.at(1).toInt()) : "")
                 + (parameters.at(0).toInt() == 1 ? db->get_object_name(RPGDB::WEAPONS, parameters.at(1).toInt()) : "")
                + (parameters.at(0).toInt() == 2 ? db->get_object_name(RPGDB::ARMORS, parameters.at(1).toInt()) : "") + "]";
    else if (code == 605)
        text += " :               : [" + (parameters.at(0).toInt() == 0 ? db->get_object_name(RPGDB::ITEMS, parameters.at(1).toInt()) : "")
                + (parameters.at(0).toInt() == 1 ? db->get_object_name(RPGDB::WEAPONS, parameters.at(1).toInt()) : "")
               + (parameters.at(0).toInt() == 2 ? db->get_object_name(RPGDB::ARMORS, parameters.at(1).toInt()) : "") + "]";
    else if (code == 303)
        text += "@>Name Input Processing: " + db->get_object_name(RPGDB::ACTORS, parameters.at(0).toInt()) + ", " + QString::number(parameters.at(1).toInt()) + " characters";
    else if (code == 311)
        text += "@>Change HP: " + (parameters.at(0).toInt() == 0 ? "Entire Party" : "[" + db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()) + "]")
                + " " + this->text_pm.at(parameters.at(1).toInt()) + " "
                + (parameters.at(2).toInt() == 0 ? QString::number(parameters.at(3).toInt()) : QString("Variable [%1: %2]").arg(parameters.at(3).toInt(),4,10, QChar('0')).arg(db->get_variable_name(parameters.at(3).toInt())));
    else if (code == 312)
        text += "@>Change SP: " + (parameters.at(0).toInt() == 0 ? "Entire Party" : "[" + db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()) + "]")
                + " " + this->text_pm.at(parameters.at(1).toInt()) + " "
                + (parameters.at(2).toInt() == 0 ? QString::number(parameters.at(3).toInt()) : QString("Variable [%1: %2]").arg(parameters.at(3).toInt(),4,10, QChar('0')).arg(db->get_variable_name(parameters.at(3).toInt())));
    else if (code == 313)
        text += "@>Change State: " + (parameters.at(0).toInt() == 0 ? "Entire Party" : "[" + db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()) + "]")
                + " " + this->text_pm.at(parameters.at(1).toInt()) + " "
                 + "[" + db->get_object_name(RPGDB::STATES,parameters.at(2).toInt()) + "]";
    else if (code == 314)
        text += "@>Recover All: " + (parameters.at(0).toInt() == 0 ? "Entire Party" : "[" + db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()) + "]");
    else if (code == 315)
        text += "@>Change EXP: " + (parameters.at(0).toInt() == 0 ? "Entire Party" : "[" + db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()) + "]")
                + " " + this->text_pm.at(parameters.at(1).toInt()) + " "
                + (parameters.at(2).toInt() == 0 ? QString::number(parameters.at(3).toInt()) : QString("Variable [%1: %2]").arg(parameters.at(3).toInt(),4,10, QChar('0')).arg(db->get_variable_name(parameters.at(3).toInt())));
    else if (code == 316)
        text += "@>Change Level: " + (parameters.at(0).toInt() == 0 ? "Entire Party" : "[" + db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()) + "]")
                + " " + this->text_pm.at(parameters.at(1).toInt()) + " "
                + (parameters.at(2).toInt() == 0 ? QString::number(parameters.at(3).toInt()) : QString("Variable [%1: %2]").arg(parameters.at(3).toInt(),4,10, QChar('0')).arg(db->get_variable_name(parameters.at(3).toInt())));
    else if (code == 317)
        text += "@>Change Parameters: " + (parameters.at(0).toInt() == 0 ? "Entire Party" : "[" + db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()) + "]")
                + ", " + text_stat_vars.at(4+parameters.at(1).toInt()) + " " + this->text_pm.at(parameters.at(2).toInt()) + " " + (parameters.at(3).toInt() == 0 ? QString::number(parameters.at(4).toInt()) : QString("Variable [%1: %2]").arg(parameters.at(4).toInt(),4,10, QChar('0')).arg(db->get_variable_name(parameters.at(4).toInt())));
        //Entire Party is not implemented here
    else if (code == 318)
        text += "@>Change Skills: " + (parameters.at(0).toInt() == 0 ? "Entire Party" : "[" + db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()) + "]")
                + " " + this->text_pm.at(parameters.at(1).toInt()) + " "
                + "[" + db->get_object_name(RPGDB::SKILLS,parameters.at(2).toInt()) + "]";
    //Entire Party is not implemented here
    else if (code == 319)
        text += "@>Change Equipment: [" + db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()) + "], " + this->text_equipment.at(parameters.at(1).toInt()) + QString(" = ")
                + (parameters.at(1).toInt() == 0 ? (parameters.at(2).toInt() == 0 ? QString("(None)") : "[" + db->get_object_name(RPGDB::WEAPONS,parameters.at(2).toInt()) + "]"): QString(""))
                + (parameters.at(1).toInt() > 0 ? (parameters.at(2).toInt() == 0 ? QString("(None)") : "[" + db->get_object_name(RPGDB::ARMORS,parameters.at(2).toInt()) + "]"): QString(""));
    else if (code == 320)
        text += "@>Change Actor Name: [" + db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()) + "], '" + parameters.at(1).toString() + "'";
    else if (code == 321)
        text += "@>Change Actor Class: [" + db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()) + "], [" + db->get_object_name(RPGDB::CLASSES,parameters.at(1).toInt()) + "]";
    else if (code == 322)
        text += "@>Change Actor Graphic: [" + db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()) + "], " + parameters.at(1).toString() + ", " + QString::number(parameters.at(2).toInt()) + ", " + parameters.at(3).toString() +", " + QString::number(parameters.at(4).toInt());
    else if (code == 331)
        text += "@>Change Enemy HP: " + (parameters.at(0).toInt() == -1 ? "Entire Troop" : "[" + QString::number(parameters.at(0).toInt()+1) + ".]")
                + " " + this->text_pm.at(parameters.at(1).toInt()) + " "
                + (parameters.at(2).toInt() == 0 ? QString::number(parameters.at(3).toInt()) : QString("Variable [%1: %2]").arg(parameters.at(3).toInt(),4,10, QChar('0')).arg(db->get_variable_name(parameters.at(3).toInt())));
    else if (code == 332)
        text += "@>Change Enemy SP: " + (parameters.at(0).toInt() == -1 ? "Entire Troop" : "[" + QString::number(parameters.at(0).toInt()+1) + ".]")
                + " " + this->text_pm.at(parameters.at(1).toInt()) + " "
                + (parameters.at(2).toInt() == 0 ? QString::number(parameters.at(3).toInt()) : QString("Variable [%1: %2]").arg(parameters.at(3).toInt(),4,10, QChar('0')).arg(db->get_variable_name(parameters.at(3).toInt())));
    else if (code == 333)
        text += "@>Change Enemy State: " + (parameters.at(0).toInt() == -1 ? "Entire Troop" : "[" + QString::number(parameters.at(0).toInt()+1) + ".]")
                + " " + this->text_pm.at(parameters.at(1).toInt()) + " "
                + "[" + db->get_object_name(RPGDB::STATES,parameters.at(2).toInt()) + "]";
    else if (code == 334)
        text += "@>Enemy Recover All: " + (parameters.at(0).toInt() == -1 ? "Entire Troop" : "[" + QString::number(parameters.at(0).toInt()+1) + ".]");
    else if (code == 335)
        text += "@>Enemy Appearance: [" + QString::number(parameters.at(0).toInt()+1) + ".]";
    else if (code == 336)
        text += "@>Enemy Transform: [" + QString::number(parameters.at(0).toInt()+1) + ".], [" + db->get_object_name(RPGDB::ENEMIES,parameters.at(1).toInt()) + "]";
    else if (code == 337)
        text += "@>Show Battle Animation: " + (parameters.at(0).toInt() == 0 ? (parameters.at(1).toInt() == -1 ? "Entire Troop, " : QString("[%1.], ").arg(parameters.at(1).toInt()+1))
                                                                             : (parameters.at(1).toInt() == -1 ? "Entire Party, " : QString("Actor No. %1,").arg(parameters.at(1).toInt()+1)))
                + "[" + db->get_object_name(RPGDB::ANIMATIONS, parameters.at(2).toInt()) + "]";
    else if (code == 338)
        text += "@>Deal Damage: " + (parameters.at(0).toInt() == 0 ? (parameters.at(1).toInt() == -1 ? "Entire Troop, " : QString("[%1.], ").arg(parameters.at(1).toInt()+1))
                                                                  : (parameters.at(1).toInt() == -1 ? "Entire Party, " : QString("Actor No. %1, ").arg(parameters.at(1).toInt()+1)))
                + (parameters.at(2).toInt() == 0 ? QString::number(parameters.at(3).toInt()) : QString("Variable [%1: %2]").arg(parameters.at(3).toInt(),4,10,QChar('0')).arg(db->get_variable_name(parameters.at(3).toInt())));
    else if (code == 339)
        text += "@>Force Action: " + (parameters.at(0).toInt() == 0 ? QString("[%1.], ").arg(parameters.at(1).toInt()+1)
                                                                  :  QString("Actor No. %1, ").arg(parameters.at(1).toInt()+1))
                + (parameters.at(2).toInt() == 0 ? this->text_basic_actions.at(parameters.at(3).toInt()) : "[" + db->get_object_name(RPGDB::SKILLS,parameters.at(3).toInt()) + "]")
                + ", " + (parameters.at(4).toInt() == -2 ? "Last Target" : (parameters.at(4).toInt() == -1 ? "Random" : QString("Index %1").arg(parameters.at(4).toInt()+1)))
                + (parameters.at(5).toInt() == 1 ? ", Execute Now" : "");
    else if (code == 340)
        text += "@>Abort Battle";
    else if (code == 351)
        text += "@>Call Menu Screen";
    else if (code == 352)
        text += "@>Call Save Screen";
    else if (code == 353)
        text += "@>Game Over";
    else if (code == 354)
        text += "@>Return to Title Screen";
    else if (code == 355)
        text += "@>Script: " + parameters.at(0).toString();
    else if (code == 655)
        text += " :      : " + parameters.at(0).toString();
    else
    {
        qDebug() << code << parameters;
        return "code: " + QString::number(code);
    }

    return text;
}
