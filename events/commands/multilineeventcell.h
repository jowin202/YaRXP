#ifndef MULTILINEEVENTCELL_H
#define MULTILINEEVENTCELL_H

#include <QObject>
#include <QWidget>
#include <QListWidgetItem>
#include <QColor>
#include <QList>

#include "RXIO/RXObjects/rpgvariant.h"
#include "RXIO/RXObjects/rpgmoveroute.h"
#include "RXIO/RXObjects/rpgeventcommand.h"

#include "simpleeventcell.h"

class MultilineEventCell : public SimpleEventCell
{
public:
    MultilineEventCell(RPGEventCommand *command, RPGSystem *system, RPGMoveCommand *mvcmd);


    void set_ref_cell(MultilineEventCell *ref_cell) {this->ref_cell = ref_cell; }

    enum {TEXT, COMMENT, SETMOVEROUTE, SCRIPT};



    bool is_head = false;
    QList<MultilineEventCell*> tail;
    MultilineEventCell *head = 0;
    RPGMoveCommand *mvcmd = 0;


private:
    MultilineEventCell *ref_cell;

};

#endif // MULTILINEEVENTCELL_H
