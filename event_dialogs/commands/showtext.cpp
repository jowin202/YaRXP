#include "showtext.h"

ShowText::ShowText(QString text, int indent, int code)
{
    this->text = text;
    this->indent = indent;
    this->code = code;
    if (code == 101)
        this->setText("@>Text : " + text);
    else if (code == 401)
        this->setText("              : " + text);
}
