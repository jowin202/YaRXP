#ifndef TILESETEDITOR_H
#define TILESETEDITOR_H

#include <QWidget>

namespace Ui {
class TilesetEditor;
}

class TilesetEditor : public QWidget
{
    Q_OBJECT

public:
    explicit TilesetEditor(QWidget *parent = nullptr);
    ~TilesetEditor();

private:
    Ui::TilesetEditor *ui;
};

#endif // TILESETEDITOR_H
