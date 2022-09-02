#include "simpleeventcell.h"


SimpleEventCell::SimpleEventCell(RPGEventCommand *command, RPGSystem *system)
{
    this->code = command->code;
    this->indent = command->indent;
    this->parameters = QList<RPGVariant>(command->parameters);

    for (int i = 0; i < command->indent; i++)
        indent_as_string += "  ";


    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    this->set_color();
    this->setFont(font);
    this->setText(indent_as_string + command->get_command_text(system));
}

void SimpleEventCell::set_color()
{
    if (code == 355 || code == 655) this->setForeground(grey);
    else if (code == 209 || code == 509) this->setForeground(dark_red);
    else if (code == 108 || code == 408) this->setForeground(green);
    else if ((code >= 112 && code <= 119) || code == 413) this->setForeground(blue);
    else if ((code >= 121 && code <= 129)) this->setForeground(red);
    else if ((code >= 131 && code <= 136)) this->setForeground(pink);
    else if (code >= 201 && code <= 208) this->setForeground(dark_red);
    else if (code >= 221 && code <= 225) this->setForeground(light_green);
    else if (code >= 231 && code <= 236) this->setForeground(dark_pink);
    else if (code >= 241 && code <= 251) this->setForeground(light_blue);
    else if ((code >= 301 && code <= 303) || (code >= 601 && code <= 605)) this->setForeground(orange);
    else this->setForeground(black);
}
