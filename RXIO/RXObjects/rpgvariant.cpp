#include "rpgvariant.h"

RPGVariant::RPGVariant()
{

}

RPGVariant::RPGVariant(int n) : QVariant(n)
{
    this->rpgstring_set = false;
}

RPGVariant::RPGVariant(bool b) : QVariant(b)
{
    this->rpgstring_set = false;
}

RPGVariant::RPGVariant(QString str) : QVariant(str)
{
    this->rpgstring_set = false;
}

RPGVariant::RPGVariant(RPGString str) : QVariant(str)
{
    this->str = str;
    this->rpgstring_set = true;
}

RPGString RPGVariant::toString()
{
    if (rpgstring_set)
        return this->str;
    else
        return ((QVariant*)this)->toString();
}
