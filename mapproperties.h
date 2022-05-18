#ifndef MAPPROPERTIES_H
#define MAPPROPERTIES_H

#include <QWidget>
#include <QDebug>

#include "parser/rpgmap.h"
#include "parser/rpgmapinfo.h"

namespace Ui {
class MapProperties;
}

class MapProperties : public QWidget
{
    Q_OBJECT

public:
    explicit MapProperties(RPGMapInfo* info, QHash<int,RPGTileset*> *tilesets, QWidget *parent = nullptr);
    ~MapProperties();
    void closeEvent(QCloseEvent *event);

private:
    Ui::MapProperties *ui;
    RPGMapInfo *mapinfo;
};

#endif // MAPPROPERTIES_H
