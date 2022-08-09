#include "rpgeventcommand.h"

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
