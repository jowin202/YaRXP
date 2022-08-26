#ifndef SIMPLEEVENTCELL_H
#define SIMPLEEVENTCELL_H

#include <QWidget>
#include <QListWidgetItem>

#include "RXIO/RXObjects/rpgsystem.h"
#include "RXIO/RXObjects/rpgeventcommand.h"

class SimpleEventCell : public QListWidgetItem
{
public:
    SimpleEventCell(RPGEventCommand *command, RPGSystem *system);

    void set_color();

    QColor black = QColor(0,0,0);
    QColor green = QColor(0x24, 0x8d, 0x23);
    QColor dark_red = QColor(0x97, 0x07, 0x0b);
    QColor red = QColor(0xff, 0x00, 0x00);
    QColor grey = QColor(0x9a, 0x9a, 0x9a);
    QColor blue = QColor(0, 0, 0xFF);
    QColor pink = QColor(0xFF, 0, 0xFF);


    int code;
    int indent;
    QList<RPGVariant> parameters;

    bool is_simple = true;
    QString indent_as_string = "";
};

#endif // SIMPLEEVENTCELL_H
