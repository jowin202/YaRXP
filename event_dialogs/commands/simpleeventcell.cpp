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
    else this->setForeground(black);
}
