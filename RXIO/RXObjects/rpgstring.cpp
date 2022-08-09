#include "rpgstring.h"


RPGString::RPGString() : QString()
{
    this->encoding = false;
}

RPGString::RPGString(QString str, bool encoding) : QString(str)
{
    this->encoding = encoding;
}
