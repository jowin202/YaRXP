#include "rpgeventcommand.h"
#include "rpgsystem.h"

RPGEventCommand::RPGEventCommand(QObject *parent) : QObject(parent)
{

}

void RPGEventCommand::debug()
{
    qDebug() << "code: " << code << " indent: " << indent << " val: " << this->getType() << " parameters: " << parameters;
}

QString RPGEventCommand::getType()
{
    if (code == 0)
        return "empty command";
    else if (code == 101)
        return "Show Text";
    else if (code == 401)
        return "Show Text Multiline";
    else if (code == 102)
        return "Show Choices";
    else if (code == 402)
        return "Show Choices Multiline";
    else if (code == 402)
        return "Show Choices Branch End";
    else if (code == 103)
        return "Input number";
    else if (code == 104)
        return "Change Text Options";
    else if (code == 105)
        return "Button Input Processing";
    else if (code == 106)
        return "Wait";
    else if (code == 108)
        return "Comment";
    else if (code == 408)
        return "Multiline Comment";
    else if (code == 111)
        return "Conditional Branch";
    else if (code == 411)
        return "Else";
    else if (code == 412)
        return "Branch End";
    else if (code == 112)
        return "Loop start";
    else if (code == 113)
        return "Break loop";
    else if (code == 413)
        return "End Loop"; //Repeat Above
    else if (code == 115)
        return "Exit Event Processing";
    else if (code == 116)
        return "Erase Event";
    else if (code == 117)
        return "Call Common Event";
    else if (code == 118)
        return "Label";
    else if (code == 119)
        return "Jump to Label";
    else if (code == 121)
        return "Control Switches";
    else if (code == 122)
        return "Control Variables";
    else if (code == 123)
        return "Control Self Switch";
    else if (code == 124)
        return "Control Timer";
    else if (code == 125)
        return "Change Gold";
    else if (code == 126)
        return "Change Items"; //Not Relevant for Pokemon
    else if (code == 127)
        return "Change Weapons"; //Not Relevant for Pokemon
    else if (code == 128)
        return "Change Armor"; //Not Relevant for Pokemon
    else if (code == 129)
        return "Change Party Member"; //Not Relevant for Pokemon
    else if (code == 131)
        return "Change Window Skin";
    else if (code == 132)
        return "Change Battle BGM";
    else if (code == 133)
        return "Change Battle End ME";
    else if (code == 134)
        return "Change Save access"; //allow to save the game
    else if (code == 135)
        return "Change Menu access"; //allow to save the game
    else if (code == 136)
        return "Change Encounter"; //can disable wild pokemon
    /**
      Starting Page 2
      **/
    else if (code == 201)
        return "Transfer Player";
    else if (code == 202)
        return "Set Event Location";
    else if (code == 203)
        return "Scroll Map";
    else if (code == 204)
        return "Change Map Settings";
    else if (code == 205)
        return "Fog Color Tone"; //BUGGY TODO
    else if (code == 206)
        return "Change Fog Opacity";
    else if (code == 207)
        return "Show Animation";
    else if (code == 208)
        return "Change Transparent Flag";
    else if (code == 209)
        return "Set Move Route"; //BUGGY TODO
    else if (code == 509)
        return "Set Move Route multiline";
    else if (code == 210)
        return "Wait for Move's Completition";
    else if (code == 221)
        return "Prepare for Transition"; //Irrelevant for Pokemon
    else if (code == 222)
        return "Execute Transition"; //Irrelevant for Pokemon
    else if (code == 223)
        return "Change Screen Color Tone"; // BUGGY TODO
    else if (code == 224) //number maybe not right
        return "Screen Flash"; // BUGGY TODO
    else if (code == 225)
        return "Screen Shake";
    else if (code == 231)
        return "Show Picture";
    else if (code == 232)
        return "Move Picture";
    else if (code == 233)
        return "Rotate Picture";
    else if (code == 234)
        return "Change Picture Color Tone"; //BUGGY TODO
    else if (code == 235)
        return "Erase Picture";
    else if (code == 236)
        return "Set Weather Effects";
    else if (code == 241)
        return "Play BGM";
    else if (code == 242)
        return "Fade out BGM";
    else if (code == 245)
        return "Play BGS";
    else if (code == 246)
        return "Fade out BGS";
    else if (code == 247)
        return "Memorize BGM/BGS";
    else if (code == 248)
        return "Restore BGM/BGS";
    else if (code == 249)
        return "Play ME";
    else if (code == 250)
        return "Play SE"; //Buggy TODO
    else if (code == 251)
        return "Stop SE";
    /**
      Starting Page 3
      **/
    else if (code == 314)
        return "Recover All";
    else if (code == 351)
        return "Call Menu Screen";
    else if (code == 352)
        return "Call Save Screen";
    else if (code == 353)
        return "Game Over";
    else if (code == 354)
        return "Return to Title Screen";
    else if (code == 355)
        return "Script";
    else if (code == 655)
        return "Script Multiline";
    /**
      Following block is irrelevant
      **/
    else if (code == 301 || code == 601 || code == 603 || code == 604)
        return "Battle Processing";
    else if (code == 302)
        return "Shop Processing";
    else if (code == 303)
        return "Name Input Processing";
    else if (code == 311)
        return "Change HP";
    else if (code == 312)
        return "Change SP";
    else if (code == 313)
        return "Change State";
    else if (code == 315)
        return "Change Exp";
    else if (code == 316)
        return "Change Level";
    else if (code == 317)
        return "Change Parameters";
    else if (code == 318)
        return "Change Skills";
    else if (code == 319)
        return "Change Equipment";
    else if (code == 320)
        return "Change Actor Name";
    else if (code == 321)
        return "Change Actor Class";
    else if (code == 322)
        return "Change Actor Graphic";
    else if (code == 331)
        return "Change Enemy HP";
    else if (code == 332)
        return "Change Enemy SP";
    else if (code == 333)
        return "Change Enemy State";
    else if (code == 334)
        return "Enemy Recover all";
    else if (code == 335)
        return "Enemy Appearance";
    else if (code == 336)
        return "Enemy Transform";
    else if (code == 337)
        return "Show battle animation";
    else if (code == 338)
        return "Deal Damage";
    else if (code == 339)
        return "Force Action";
    else if (code == 340)
        return "Abort Battle";
    else if (code == 403)
        return "When Cancel";
    else if (code == 404)
        return "Branch End";
    else
        return "unknown (" + QString::number(code) + ")";
}

QString RPGEventCommand::get_command_text(RPGSystem *system)
{
    if (code == 0)
        return "@>";
    else if (code == 101)
        return "@>Text : " + parameters.at(0).str;
    else if (code == 401)
        return "       : " + parameters.at(0).str;
    else if (code == 102)
        return "TODO Show Choices";
    else if (code == 402)
        return "TODO Show Choices Multiline";
    else if (code == 402)
        return "TODO Show Choices Branch End";
    else if (code == 103)
    {
        QString num = QString::number(parameters.at(0).toInt()).rightJustified(4,'0');
        QString var = system->variable_names.at(parameters.at(0).toInt()-1);
        return "@>Input Number: [" + num + ": " + var + "], " + QString::number(parameters.at(1).toInt()) + " digit(s)";
    }
    else if (code == 104)
    {
        QStringList pos;
        pos << "Top";
        pos << "Middle";
        pos << "Bottom";

        QStringList win;
        win << "Show";
        win << "Hide";

        return "@>Change Text Options: " + pos[parameters.at(0).toInt()] + ", " + win[parameters.at(1).toInt()];
    }
    else if (code == 105)
    {
        QString num = QString::number(parameters.at(0).toInt()).rightJustified(4,'0');
        QString var = system->variable_names.at(parameters.at(0).toInt()-1);
        return "@>Button Input Processing: [" + num + ": " + var + "]";
    }
    else if (code == 106)
        return "@>Wait: " + QString::number(parameters.at(0).toInt()) + " frame(s)";
    else if (code == 108)
        return "@>Comment : " + parameters.at(0).str;
    else if (code == 408)
        return "          : " + parameters.at(0).str;
    else if (code == 111)
        return "TODO Conditional Branch";
    else if (code == 411)
        return "Else";
    else if (code == 412)
        return "Branch End";
    else if (code == 112)
        return "@>Loop";
    else if (code == 113)
        return "@>Break loop";
    else if (code == 413)
        return "Repeat above"; //Repeat Above
    else if (code == 115)
        return "@>Exit Event Processing";
    else if (code == 116)
        return "@>Erase Event";
    else if (code == 117)
    {
        QString common_event_name = "";
        if (system->common_events_list.length() >= parameters.at(0).toInt())
            common_event_name = system->common_events_list.at(parameters.at(0).toInt()-1)->name;
        return "@>Call Common Event: " + common_event_name;
    }
    else if (code == 118)
        return "@>Label: " + parameters.at(0).str;
    else if (code == 119)
        return "@>Jump to Label: " + parameters.at(0).str;
    else if (code == 121)
    {
        QString status = (parameters.at(2).toInt() == 0 ? "ON" : "OFF");

        if (parameters.at(0).toInt() == parameters.at(1).toInt())
        {
            QString num = QString::number(parameters.at(0).toInt()).rightJustified(4,'0');
            return "@>Control Switches: [" + num + ": " + system->switches_names.at(parameters.at(0).toInt()-1) + "] = " + status;
        }
        else
        {
            QString num1 = QString::number(parameters.at(0).toInt()).rightJustified(4,'0');
            QString num2 = QString::number(parameters.at(1).toInt()).rightJustified(4,'0');
            return "@>Control Switches: [" + num1 + ".." + num2 + "] = " + status;
        }
    }
    else if (code == 122)
    {
        int var_from = parameters.at(0).toInt();
        int var_to = parameters.at(1).toInt();
        int operation = parameters.at(2).toInt();
        int operand_type = parameters.at(3).toInt();

        QString var;
        if (var_from == var_to)
            var = "[" + QString::number(var_from).rightJustified(4,'0') + ": " + system->variable_names.at(var_from-1) + "] ";
        else
            var = "[" + QString::number(var_from).rightJustified(4,'0') + ".." + QString::number(var_to).rightJustified(4,'0') + "] ";

        QString op = "= ";
        if (operation == 1) op = "+= ";
        else if (operation == 2) op = "-= ";
        else if (operation == 3) op = "*= ";
        else if (operation == 4) op = "/= ";
        else if (operation == 5) op = "%= ";

        QString op2 = "";
        if (operand_type == 0)  //constant
            op2 = QString::number(parameters.at(4).toInt());
        else if (operand_type == 1) // variable
            op2 = "Variable [" + QString::number(parameters.at(4).toInt()).rightJustified(4,'0') + ": " + system->variable_names.at(parameters.at(4).toInt()-1) + "]";
        else if (operand_type == 2) // random
            op2 = "Random No. (" + QString::number(parameters.at(4).toInt()) +".." + QString::number(parameters.at(5).toInt()) +")";
        else if (operand_type == 3) // item
            op2 = "[" + system->items_list.at(parameters.at(4).toInt()-1)->name + "] In Inventory";
        else if (operand_type == 4) // actor
        {
            int s = parameters.at(5).toInt();
            QString status_var;
            if (s == 0) status_var = "Level";
            else if (s == 1) status_var = "EXP";
            else if (s == 2) status_var = "HP";
            else if (s == 3) status_var = "SP";
            else if (s == 4) status_var = "MaxHP";
            else if (s == 5) status_var = "MaxSP";
            else if (s == 6) status_var = "STR";
            else if (s == 7) status_var = "DEX";
            else if (s == 8) status_var = "AGI";
            else if (s == 9) status_var = "INT";
            else if (s == 10) status_var = "ATK";
            else if (s == 11) status_var = "PDEF";
            else if (s == 12) status_var = "MDEF";
            else if (s == 13) status_var = "EVA";
            op2 = "[" + system->actor_list.at(parameters.at(4).toInt()-1)->name + "]'s " + status_var;
        }
        else if (operand_type == 5) // enemies
        {
            int s = parameters.at(5).toInt();
            QString status_var;
            if (s == 0) status_var = "HP";
            else if (s == 1) status_var = "SP";
            else if (s == 2) status_var = "MaxHP";
            else if (s == 3) status_var = "MaxSP";
            else if (s == 4) status_var = "STR";
            else if (s == 5) status_var = "DEX";
            else if (s == 6) status_var = "AGI";
            else if (s == 7) status_var = "INT";
            else if (s == 8) status_var = "ATK";
            else if (s == 9) status_var = "PDEF";
            else if (s == 10) status_var = "MDEF";
            else if (s == 11) status_var = "EVA";
            op2 = "[" + QString::number(parameters.at(4).toInt()+1) + "]'s " + status_var;
            /**
              TODO: Original RPGMaker XP might be bugged here.
              Let's make it better
              **/
        }
        else if (operand_type == 6) // Characters
        {
            int v = parameters.at(4).toInt();
            QString name;
            if (v == -1) name = "Player";
            else if (v == 0) name = "This event";
            else {
                QList<RPGEvent*> list = system->get_current_map_info()->map->events;
                for (int i = 0; i < list.length(); i++)
                {
                    if (list.at(i)->id == v)
                        name = "[" + list.at(i)->name + "]";
                }
            }
            int s = parameters.at(5).toInt();
            QString status_var;
            if (s == 0) status_var = "Map X";
            else if (s == 1) status_var = "Map Y";
            else if (s == 2) status_var = "Direction";
            else if (s == 3) status_var = "Screen X";
            else if (s == 4) status_var = "Screen Y";
            else if (s == 5) status_var = "Terrain Tag";
            op2 = name +"'s " + status_var;

        }
        else if (operand_type == 7) // Others
        {
            int s = parameters.at(4).toInt();
            if (s == 0) op2 = "Map ID";
            else if (s == 1) op2 = "Party Members";
            else if (s == 2) op2 = "Gold";
            else if (s == 3) op2 = "Steps";
            else if (s == 4) op2 = "Play Time";
            else if (s == 5) op2 = "Timer";
            else if (s == 6) op2 = "Save Count";

        }
        return "@>Control Variables: " + var + op + op2;
    }
    else if (code == 123)
        return "@>Control Self Switch " + parameters.at(0).str + (parameters.at(1).toInt() == 0 ? " = ON" : " = OFF");
    else if (code == 124)
    {
        QString text;
        if (parameters.at(0).toInt() == 0)
            text = QString("Startup (") + QString::number(parameters.at(1).toInt()/60) + " min., " + QString::number(parameters.at(1).toInt()%60) + " sec.)";
        else text = "Stop";
        return "@>Control Timer: " + text;
    }
    else if (code == 125)
    {
        QString plus_minus = (parameters.at(0).toInt() == 0 ? "+ " : "- ");
        QString var;
        if (parameters.at(1).toInt() == 0)
            var = QString::number(parameters.at(2).toInt());
        else
            var = "Variable [" + QString::number(parameters.at(2).toInt()).rightJustified(4,'0') + ": " + system->variable_names.at(parameters.at(2).toInt()-1) + "]";
        return "@>Change Gold: " + plus_minus + var;
    }
    else if (code == 126)
    {
        QString item = "[" + system->items_list.at(parameters.at(0).toInt()-1)->name + "], ";
        QString plus_minus = (parameters.at(1).toInt() == 0 ? "+ " : "- ");

        QString var;
        if (parameters.at(2).toInt() == 0)
            var = QString::number(parameters.at(3).toInt());
        else
            var = "Variable [" + QString::number(parameters.at(3).toInt()).rightJustified(4,'0') + ": " + system->variable_names.at(parameters.at(3).toInt()-1) + "]";
        return "@>Change Items: " + item + plus_minus + var;
    }
    else if (code == 127)
    {
        QString weapon = "[" + system->weapons_list.at(parameters.at(0).toInt()-1)->name + "], ";
        QString plus_minus = (parameters.at(1).toInt() == 0 ? "+ " : "- ");

        QString var;
        if (parameters.at(2).toInt() == 0)
            var = QString::number(parameters.at(3).toInt());
        else
            var = "Variable [" + QString::number(parameters.at(3).toInt()).rightJustified(4,'0') + ": " + system->variable_names.at(parameters.at(3).toInt()-1) + "]";
        return "@>Change Weapons: " + weapon + plus_minus + var;
    }
    else if (code == 128)
    {
        QString armor = "[" + system->armors_list.at(parameters.at(0).toInt()-1)->name + "], ";
        QString plus_minus = (parameters.at(1).toInt() == 0 ? "+ " : "- ");

        QString var;
        if (parameters.at(2).toInt() == 0)
            var = QString::number(parameters.at(3).toInt());
        else
            var = "Variable [" + QString::number(parameters.at(3).toInt()).rightJustified(4,'0') + ": " + system->variable_names.at(parameters.at(3).toInt()-1) + "]";
        return "@>Change Armors: " + armor + plus_minus + var;
    }
    else if (code == 129)
    {
        QString actor = " [" + system->actor_list.at(parameters.at(0).toInt()-1)->name + "]";
        QString action = parameters.at(1).toInt() == 0 ? "Add" : "Remove";

        return "@>Change Party Member: " + action + actor + (parameters.at(1).toInt() == 0 && parameters.at(2).toInt() == 1 ? ", Initlialize" : "");
    }
    else if (code == 131)
        return "@>Change Window Skin: '" + parameters.at(0).str + "'";
    else if (code == 132)
        return "@>Change Battle BGM: '" + audiofile.name + "', " + QString::number(audiofile.volume) + ", " + QString::number(audiofile.pitch);
    else if (code == 133)
        return "@>Change Battle End ME: '" + audiofile.name + "', " + QString::number(audiofile.volume) + ", " + QString::number(audiofile.pitch);
    else if (code == 134)
        return "Change Save access: " + QString(parameters.at(0).toInt() == 0 ? "Disable":"Enable");
    else if (code == 135)
        return "@>Change Menu access: " + QString(parameters.at(0).toInt() == 0 ? "Disable":"Enable");
    else if (code == 136)
        return "@>Change Encounter: " + QString(parameters.at(0).toInt() == 0 ? "Disable":"Enable");
    /**
      Starting Page 2
      **/
    else if (code == 201)
    {
        QString data;
        if (parameters.at(0).toInt() == 0)
        {
            int map_num = parameters.at(1).toInt();
            for (int i = 0; i < system->map_info_list.length(); i++)
            {
                if (system->map_info_list.at(i)->id == map_num)
                        data = "[" + QString::number(map_num).rightJustified(3,'0') + ": " + system->map_info_list.at(i)->name + "],(" + QString::number(parameters.at(2).toInt()).rightJustified(3,'0') + "," +  QString::number(parameters.at(3).toInt()).rightJustified(3,'0') + ")";
            }
        }
        else
        {
            data = "Variable [" + QString::number(parameters.at(1).toInt()).rightJustified(3,'0') + "]["+ QString::number(parameters.at(2).toInt()).rightJustified(3,'0') + "]["+ QString::number(parameters.at(3).toInt()).rightJustified(3,'0') + "]";
        }


        switch (parameters.at(4).toInt())
        {
        case 2: data.append(", Down"); break;
        case 4: data.append(", Left"); break;
        case 6: data.append(", Right"); break;
        case 8: data.append(", Up"); break;
        }
        if (parameters.at(5).toInt() == 1)
            data.append(", No Fade");
        return "@>Transfer Player: " + data;
    }
    else if (code == 202)
    {
        QString data;
        if (parameters.at(0).toInt() == 0) data = "This event";
        else
            data = "[" + system->get_current_map_info()->map->event_by_id(parameters.at(0).toInt())->name + "]";

        if (parameters.at(1).toInt() == 0) //direct appointment
            data += ", (" + QString::number(parameters.at(2).toInt()).rightJustified(3,'0') + "," + QString::number(parameters.at(3).toInt()).rightJustified(3,'0') + ")";
        else if (parameters.at(1).toInt() == 1) //variable
            data += ", Variable [" + QString::number(parameters.at(2).toInt()).rightJustified(4,'0') + "][" + QString::number(parameters.at(3).toInt()).rightJustified(4,'0') + "]";
        else if (parameters.at(1).toInt() == 2) //switch with
            data += ", Switch with [" + system->get_current_map_info()->map->event_by_id(parameters.at(2).toInt())->name + "]";

        switch (parameters.at(4).toInt())
        {
        case 2: data.append(", Down"); break;
        case 4: data.append(", Left"); break;
        case 6: data.append(", Right"); break;
        case 8: data.append(", Up"); break;
        }

        return "@>Set Event Location: " + data;
    }
    else if (code == 203)
    {
        QString data;
        switch (parameters.at(0).toInt())
        {
        case 2: data.append("Down"); break;
        case 4: data.append("Left"); break;
        case 6: data.append("Right"); break;
        case 8: data.append("Up"); break;
        }

        data += ", " + QString::number(parameters.at(1).toInt()) + ", " + QString::number(parameters.at(2).toInt());

        return "@>Scroll Map: " + data;
    }
    else if (code == 204)
    {
        QString data;
        if (parameters.at(0).toInt() == 0)
        {
            data = "Panorama = ";
            data += "'" + parameters.at(1).str + "', " + QString::number(parameters.at(2).toInt());
        }
        else if (parameters.at(0).toInt() == 1)
        {
                data = "Fog = ";
                data += "'" + parameters.at(1).str + "', " + QString::number(parameters.at(2).toInt()) + ", " + QString::number(parameters.at(3).toInt());
                switch (parameters.at(4).toInt())
                {
                case 0: data.append(", Normal"); break;
                case 1: data.append(", Add"); break;
                case 2: data.append(", Sub"); break;
                }
                data += ", " + QString::number(parameters.at(5).toInt());
                data += ", " + QString::number(parameters.at(6).toInt());
                data += ", " + QString::number(parameters.at(7).toInt());
        }
        else if (parameters.at(0).toInt() == 2)
        {
                data = "Battleback = ";
                data += "'" + parameters.at(1).str + "'";
        }

        return "@>Change Map Settings: " + data;
    }
    else if (code == 205)
        return "@>Fog Color Tone: (" + QString::number(this->red) + ","+ QString::number(this->green) + ","+ QString::number(this->blue) + ","+ QString::number(this->gray) + "), @" + QString::number(parameters.at(0).toInt());
    else if (code == 206)
        return "@>Change Fog Opacity: " + QString::number(parameters.at(0).toInt()) + ", @" + QString::number(parameters.at(1).toInt());
    else if (code == 207)
    {
        QString data;
        if (parameters.at(0).toInt() == -1)
            data = "Player";
        else if (parameters.at(0).toInt() == 0)
            data = "This event";
        else
            data = "[" + system->get_current_map_info()->map->event_by_id(parameters.at(0).toInt())->name + "]";

        data += ", ";
        data += "[" + system->animation_list.at(parameters.at(1).toInt()-1)->name + "]";
        return "@>Show Animation: " + data;
    }
    else if (code == 208)
        return "@>Change Transparent Flag: " + QString(parameters.at(0).toInt() == 0 ? "Transparent" : "Normal");
    else if (code == 209)
        return "Set Move Route";
    else if (code == 509)
        return "NOT USED HERE";
    else if (code == 210)
        return "@>Wait for Move's Completition";
    else if (code == 221)
        return "@>Prepare for Transition";
    else if (code == 222)
        return "@>Execute Transition: '" + parameters.at(0).str + "'";
    else if (code == 223)
        return "@>Change Screen Color Tone: (" + QString::number(this->red) + ","+ QString::number(this->green) + ","+ QString::number(this->blue) + ","+ QString::number(this->gray) + "), @" + QString::number(parameters.at(0).toInt());
    else if (code == 224) //number maybe not right
        return "@>Screen Flash: (" + QString::number(this->red) + ","+ QString::number(this->green) + ","+ QString::number(this->blue) + ","+ QString::number(this->gray) + "), @" + QString::number(parameters.at(0).toInt());
    else if (code == 225)
        return "@>Screen Shake: " + QString::number(parameters.at(0).toInt()) + ", " + QString::number(parameters.at(1).toInt()) + ", @" + QString::number(parameters.at(2).toInt());
    else if (code == 231)
    {
        QString data;
        data.append(QString::number(parameters.at(0).toInt()));
        data.append(", '" + parameters.at(1).str + "', ");
        data.append(parameters.at(2).toInt() == 0 ? "Upper Left " : "Center ");
        if (parameters.at(3).toInt() == 0) //constant coordinates
            data.append("(" + QString::number(parameters.at(4).toInt()) + "," + QString::number(parameters.at(5).toInt()) + "), ");
        else //variables
            data.append("(Variable [" + QString::number(parameters.at(4).toInt()).rightJustified(4,'0') + "],[" + QString::number(parameters.at(5).toInt()).rightJustified(4,'0') + "]), ");

        //Zoom
        data.append("(" + QString::number(parameters.at(6).toInt()) + "\%," + QString::number(parameters.at(7).toInt()) + "\%)");

        data.append(", " + QString::number(parameters.at(8).toInt()) + ", "); //opacity

        //Blending
        if (parameters.at(9).toInt() == 0)
            data.append("Normal");
        else if (parameters.at(9).toInt() == 1)
            data.append("Add");
        else //if (parameters.at(9).toInt() == 2)
            data.append("Sub");

        return "@>Show Picture: " + data;
    }
    else if (code == 232)
    {
        QString data;
        data.append(QString::number(parameters.at(0).toInt()));
        data.append(", @" + QString::number(parameters.at(1).toInt()) + ", ");
        data.append(parameters.at(2).toInt() == 0 ? "Upper Left " : "Center ");

        if (parameters.at(3).toInt() == 0) //constant coordinates
            data.append("(" + QString::number(parameters.at(4).toInt()) + "," + QString::number(parameters.at(5).toInt()) + "), ");
        else //variables
            data.append("(Variable [" + QString::number(parameters.at(4).toInt()).rightJustified(4,'0') + "],[" + QString::number(parameters.at(5).toInt()).rightJustified(4,'0') + "]), ");

        data.append("(" + QString::number(parameters.at(6).toInt()) + "\%," + QString::number(parameters.at(7).toInt()) + "\%)");
        data.append(", " + QString::number(parameters.at(8).toInt()) + ", "); //opacity

        //Blending
        if (parameters.at(9).toInt() == 0)
            data.append("Normal");
        else if (parameters.at(9).toInt() == 1)
            data.append("Add");
        else //if (parameters.at(9).toInt() == 2)
            data.append("Sub");

        return "@>Move Picture: " + data;
    }
    else if (code == 233)
    {
        QString data;
        QString plus = (parameters.at(1).toInt() > 0 ? "+" : "");
        data.append(QString::number(parameters.at(0).toInt()));
        data.append(", " + plus + QString::number(parameters.at(1).toInt()));
        return "@>Rotate Picture: " + data;
    }
    else if (code == 234)
    {
        QString data;
        data.append(QString::number(parameters.at(0).toInt()) + ", (");
        data.append(QString::number(this->red) + ",");
        data.append(QString::number(this->green) + ",");
        data.append(QString::number(this->blue) + ",");
        data.append(QString::number(this->gray) + "), @");
        data.append(QString::number(parameters.at(1).toInt()));

        return "@>Change Picture Color Tone: " + data;
    }
    else if (code == 235)
        return "@>Erase Picture: " + QString::number(parameters.at(0).toInt());
    else if (code == 236)
    {
        QString data;
        if (parameters.at(0).toInt() == 0)
            data.append("None");
        else if (parameters.at(0).toInt() == 1)
            data.append("Rain");
        else if (parameters.at(0).toInt() == 2)
            data.append("Storm");
        else //if (parameters.at(0).toInt() == 3)
            data.append("Snow");

        data.append(", " + QString::number(parameters.at(1).toInt()) + ", @");
        data.append(QString::number(parameters.at(2).toInt()));

        return "@>Set Weather Effects: " + data;
    }
    else if (code == 241)
        return "@>Play BGM: '" + this->audiofile.name + "', " + QString::number(this->audiofile.volume) + ", " + QString::number(this->audiofile.pitch);
    else if (code == 242)
        return "@>Fade out BGM: " + QString::number(parameters.at(0).toInt()) + " sec.";
    else if (code == 245)
        return "@>Play BGS: '" + this->audiofile.name + "', " + QString::number(this->audiofile.volume) + ", " + QString::number(this->audiofile.pitch);
    else if (code == 246)
        return "@>Fade out BGS: " + QString::number(parameters.at(0).toInt()) + " sec.";
    else if (code == 247)
        return "@>Memorize BGM/BGS";
    else if (code == 248)
        return "@>Restore BGM/BGS";
    else if (code == 249)
        return "@>Play ME: '" + this->audiofile.name + "', " + QString::number(this->audiofile.volume) + ", " + QString::number(this->audiofile.pitch);
    else if (code == 250)
        return "@>Play SE: '" + this->audiofile.name + "', " + QString::number(this->audiofile.volume) + ", " + QString::number(this->audiofile.pitch);
    else if (code == 251)
        return "@>Stop SE";
    /**
      Starting Page 3
      **/


    else if (code == 301 || code == 601 || code == 603 || code == 604)
        return "Battle Processing";
    else if (code == 302)
    {
        QString data;
        if (parameters.at(0).toInt() == 0) // item
            data.append(system->items_list.at(parameters.at(1).toInt()-1)->name);
        else if (parameters.at(0).toInt() == 1) // weapon
            data.append(system->weapons_list.at(parameters.at(1).toInt()-1)->name);
        else if (parameters.at(0).toInt() == 2) // armor
            data.append(system->armors_list.at(parameters.at(1).toInt()-1)->name);
        data.append("]");
        return "@>Shop Processing : [" + data;
    }
    else if (code == 605)
    {
        QString data;
        if (parameters.at(0).toInt() == 0) // item
            data.append(system->items_list.at(parameters.at(1).toInt()-1)->name);
        else if (parameters.at(0).toInt() == 1) // weapon
            data.append(system->weapons_list.at(parameters.at(1).toInt()-1)->name);
        else if (parameters.at(0).toInt() == 2) // armor
            data.append(system->armors_list.at(parameters.at(1).toInt()-1)->name);
        data.append("]");
        return "                  : [" + data;
    }
    else if (code == 303)
        return "@>Name Input Processing: " + system->actor_list.at(parameters.at(0).toInt()-1)->name + ", " + QString::number(parameters.at(1).toInt()) + " characters";
    else if (code == 311)
        return "Change HP";
    else if (code == 312)
        return "Change SP";
    else if (code == 313)
        return "Change State";
    else if (code == 314)
        return "Recover All";
    else if (code == 315)
        return "Change Exp";
    else if (code == 316)
        return "Change Level";
    else if (code == 317)
        return "Change Parameters";
    else if (code == 318)
        return "Change Skills";
    else if (code == 319)
        return "Change Equipment";
    else if (code == 320)
        return "Change Actor Name";
    else if (code == 321)
        return "Change Actor Class";
    else if (code == 322)
        return "Change Actor Graphic";
    else if (code == 331)
        return "Change Enemy HP";
    else if (code == 332)
        return "Change Enemy SP";
    else if (code == 333)
        return "Change Enemy State";
    else if (code == 334)
        return "Enemy Recover all";
    else if (code == 335)
        return "Enemy Appearance";
    else if (code == 336)
        return "Enemy Transform";
    else if (code == 337)
        return "Show battle animation";
    else if (code == 338)
        return "Deal Damage";
    else if (code == 339)
        return "Force Action";
    else if (code == 340)
        return "Abort Battle";

    else if (code == 351)
        return "Call Menu Screen";
    else if (code == 352)
        return "Call Save Screen";
    else if (code == 353)
        return "Game Over";
    else if (code == 354)
        return "Return to Title Screen";
    else if (code == 355)
        return "@>Script : " + parameters.at(0).str;
    else if (code == 655)
        return "         : " + parameters.at(0).str;

    else if (code == 403)
        return "When Cancel";
    else if (code == 404)
        return "Branch End";
    else
        return "unknown (" + QString::number(code) + ")";
}

