#ifndef RPGVARIANT_H
#define RPGVARIANT_H

#include <QObject>
#include <QVariant>

#include "rpgstring.h"

class RPGVariant : public QVariant
{

public:
    RPGVariant();
    RPGVariant(int n);
    RPGVariant(bool b);
    RPGVariant(QString str);
    RPGVariant(RPGString str);

    RPGString toString();


    bool rpgstring_set;
    RPGString str;
};

#endif // RPGVARIANT_H
