#include "showtext.h"

ShowText::ShowText(QString text, int indent, int code)
{
    this->text = text;
    this->indent = indent;
    this->code = code;
    this->setText("@>Text : " + text);
}
