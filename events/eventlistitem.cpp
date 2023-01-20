#include "eventlistitem.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgmapcontroller.h"
#include "RXIO2/rpgmapinfocontroller.h"


EventListItem::EventListItem(QListWidget *parent, RPGMapController *mc, RPGMapInfoController *mic, QJsonObject obj) : QListWidgetItem(parent)
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


    if (code == 106)
    {
        bool ok;
        int v = QInputDialog::getInt(this->parent, "Wait", "Frames:", parameters.at(0).toInt(),1,999, 1, &ok);
        if (ok){
            QJsonArray newparams;
            newparams.append(v);
            this->obj.insert("@parameters", newparams);
        }
    }
    if (code == 118 || code == 119)
    {
        bool ok;
        QString label = QInputDialog::getText(this->parent, (code == 119 ? QString("Jump to ") : "") + "Label", (code == 119 ? QString("Jump to ") : "") + "Label:", QLineEdit::Normal, parameters.at(0).toString(), &ok);
        if (ok){
            QJsonArray newparams;
            newparams.append(label);
            this->obj.insert("@parameters", newparams);
        }
    }
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
        text = "@>Wait: " + parameters.at(0).toString();
    else if (code == 408)
        text = " :    : " + parameters.at(0).toString();
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
                           : QString("Variable [%1: %2]").arg(parameters.at(3).toInt()).arg(db->get_variable_name(parameters.at(3).toInt()))); break; // variable
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
                             "is appeared" : QString("[%1] inflicted").arg(db->get_object_name(RPGDB::STATES,parameters.at(3).toInt()))); break; // enemy
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
        text = "@>Repeat Above";
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
                ((parameters.at(0) == parameters.at(1)) ? QString("[%1: %2] %3 ")
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
                  (parameters.at(1).toInt() == 1 ? QString("Variable [%1,%2]").arg(parameters.at(2).toInt(),4,10,QChar('0')).arg(parameters.at(3).toInt(),4,10,QChar('0')) :
                                                   QString("Switch with [%1]").arg(mc->current_map()->object().value("@events").toObject().value(QString::number(parameters.at(2).toInt())).toObject().value("@name").toString())))
                + (parameters.at(4).toInt() != 0 ? ", " +this->text_directions.at(parameters.at(4).toInt()/2-1) : "");
    else if (code == 203)
        text += "@>Scroll Map: " + this->text_directions.at(parameters.at(0).toInt()/2-1) + ", " + QString::number(parameters.at(1).toInt()) + ", " + QString::number(parameters.at(2).toInt());
    else if (code == 204)
        text += "@>Change Map Setings: " + (parameters.at(0).toInt() == 0 ?  QString("Panorama = '%1', %2").arg(parameters.at(1).toString()).arg(parameters.at(2).toInt()) :
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
        text += "@>Change Screen Color Tone: " + QString("(%1, %2, %3, %4), @%5")
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
        text += "@>Change Picture Color Tone: " + QString("%1, (%2, %3, %4, %5), @%6")
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
        text += " :  If Win";
    else if (code == 603)
        text += " :  If Lose";
    else if (code == 604)
        text += " :  Branch End";
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
                + (parameters.at(2).toInt() == 0 ? QString::number(parameters.at(3).toInt()) : QString("Variable [%1, %2]").arg(parameters.at(3).toInt(),4,10, QChar('0')).arg(db->get_variable_name(parameters.at(3).toInt())));
    else if (code == 312)
        text += "@>Change SP: " + (parameters.at(0).toInt() == 0 ? "Entire Party" : "[" + db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()) + "]")
                + " " + this->text_pm.at(parameters.at(1).toInt()) + " "
                + (parameters.at(2).toInt() == 0 ? QString::number(parameters.at(3).toInt()) : QString("Variable [%1, %2]").arg(parameters.at(3).toInt(),4,10, QChar('0')).arg(db->get_variable_name(parameters.at(3).toInt())));
    else if (code == 313)
        text += "@>Change State: " + (parameters.at(0).toInt() == 0 ? "Entire Party" : "[" + db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()) + "]")
                + " " + this->text_pm.at(parameters.at(1).toInt()) + " "
                 + "[" + db->get_object_name(RPGDB::STATES,parameters.at(2).toInt()) + "]";
    else if (code == 314)
        text += "@>Recover All: " + (parameters.at(0).toInt() == 0 ? "Entire Party" : "[" + db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()) + "]");
    else if (code == 315)
        text += "@>Change EXP: " + (parameters.at(0).toInt() == 0 ? "Entire Party" : "[" + db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()) + "]")
                + " " + this->text_pm.at(parameters.at(1).toInt()) + " "
                + (parameters.at(2).toInt() == 0 ? QString::number(parameters.at(3).toInt()) : QString("Variable [%1, %2]").arg(parameters.at(3).toInt(),4,10, QChar('0')).arg(db->get_variable_name(parameters.at(3).toInt())));
    else if (code == 316)
        text += "@>Change Level: " + (parameters.at(0).toInt() == 0 ? "Entire Party" : "[" + db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()) + "]")
                + " " + this->text_pm.at(parameters.at(1).toInt()) + " "
                + (parameters.at(2).toInt() == 0 ? QString::number(parameters.at(3).toInt()) : QString("Variable [%1, %2]").arg(parameters.at(3).toInt(),4,10, QChar('0')).arg(db->get_variable_name(parameters.at(3).toInt())));
    else if (code == 317)
        text += "@>Change Parameters: " + (parameters.at(0).toInt() == 0 ? "Entire Party" : "[" + db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()) + "]")
                + ", " + text_stat_vars.at(4+parameters.at(1).toInt()) + " " + this->text_pm.at(parameters.at(2).toInt()) + " " + (parameters.at(3).toInt() == 0 ? QString::number(parameters.at(4).toInt()) : QString("Variable [%1, %2]").arg(parameters.at(4).toInt(),4,10, QChar('0')).arg(db->get_variable_name(parameters.at(4).toInt())));
        //Entire Party is not implemented here
    else if (code == 318)
        text += "@>Change Skills: " + (parameters.at(0).toInt() == 0 ? "Entire Party" : "[" + db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()) + "]")
                + " " + this->text_pm.at(parameters.at(1).toInt()) + " "
                + "[" + db->get_object_name(RPGDB::SKILLS,parameters.at(2).toInt()) + "]";
    //Entire Party is not implemented here
    else if (code == 320)
        text += "@>Change Actor Name: [" + db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()) + "], '" + parameters.at(1).toString() + "'";
    else if (code == 321)
        text += "@>Change Actor Class: [" + db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()) + "], [" + db->get_object_name(RPGDB::CLASSES,parameters.at(1).toInt()) + "]";
    else if (code == 322)
        text += "@>Change Actor Graphic: [" + db->get_object_name(RPGDB::ACTORS,parameters.at(0).toInt()) + "], " + parameters.at(1).toString() + ", " + QString::number(parameters.at(2).toInt()) + ", " + parameters.at(3).toString() +", " + QString::number(parameters.at(4).toInt());
    else if (code == 331)
        text += "@>Change Enemy HP: " + (parameters.at(0).toInt() == -1 ? "Entire Troop" : "[" + QString::number(parameters.at(0).toInt()+1) + ".]")
                + " " + this->text_pm.at(parameters.at(1).toInt()) + " "
                + (parameters.at(2).toInt() == 0 ? QString::number(parameters.at(3).toInt()) : QString("Variable [%1, %2]").arg(parameters.at(3).toInt(),4,10, QChar('0')).arg(db->get_variable_name(parameters.at(3).toInt())));
    else if (code == 332)
        text += "@>Change Enemy SP: " + (parameters.at(0).toInt() == -1 ? "Entire Troop" : "[" + QString::number(parameters.at(0).toInt()+1) + ".]")
                + " " + this->text_pm.at(parameters.at(1).toInt()) + " "
                + (parameters.at(2).toInt() == 0 ? QString::number(parameters.at(3).toInt()) : QString("Variable [%1, %2]").arg(parameters.at(3).toInt(),4,10, QChar('0')).arg(db->get_variable_name(parameters.at(3).toInt())));
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
