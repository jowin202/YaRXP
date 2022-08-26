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
        return "@>Script : " + parameters.at(0).str;
    else if (code == 655)
        return "         : " + parameters.at(0).str;
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

