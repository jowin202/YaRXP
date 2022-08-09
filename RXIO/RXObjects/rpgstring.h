#ifndef RPGSTRING_H
#define RPGSTRING_H

#include <QString>

class RPGString : public QString
{
public:
    RPGString();
    RPGString(QString str, bool encoding = false);

    bool encoding;

};

#endif // RPGSTRING_H
