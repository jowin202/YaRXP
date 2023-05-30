#ifndef IMPORTSCREENSHOTDIALOG_H
#define IMPORTSCREENSHOTDIALOG_H

#include <QWidget>
#include <QFileDialog>
#include <QSettings>
#include <QPainter>
#include <QGraphicsItem>

class RPGDB;

namespace Ui {
class ImportScreenshotDialog;
}

class ImportScreenshotDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ImportScreenshotDialog(RPGDB *db, QWidget *parent = nullptr);
    ~ImportScreenshotDialog();


signals:
    void finished();

private slots:
    void on_button_import_clicked();
    void on_combo_tileset_currentIndexChanged(int index);
    void on_button_browse_img_clicked();

    void load_screenshot();


    void on_button_similartiles_clicked();

    void on_button_show_similar_tiles_clicked();

private:
    Ui::ImportScreenshotDialog *ui;
    RPGDB *db;
    QImage orig_img;
    QImage edited_img;
    QMap<QPair<int,int>, int> similar_tiles;
};

#endif // IMPORTSCREENSHOTDIALOG_H
