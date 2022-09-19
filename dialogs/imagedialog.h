#ifndef IMAGEDIALOG_H
#define IMAGEDIALOG_H

#include <QWidget>
#include <QDir>
#include <QDirIterator>
#include <QKeyEvent>

namespace Ui {
class ImageDialog;
}

class RPGSystem;
class ImageDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ImageDialog(RPGSystem *system, int mode, QString current_image_file, QWidget *parent = nullptr);
    ~ImageDialog();

    enum { EVENTPAGE, ANIMATIONS, AUTOTILES, BATTLEBACKS, BATTLERS, CHARACTERS, FOGS, ICONS, PANOMRAMAS, PICTURES, TILESETS, TITLES, TRANSITIONS, WINDOWSKINS};

    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->close();
        else if(e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }


signals:
    void ok_clicked(QString);
    //void ok_clicked_extended(QString, int, int, int, int, int, int);

public slots:
    void update_image_list();

private slots:
    void on_button_ok_clicked();
    void on_button_cancel_clicked();

    void on_list_currentRowChanged(int currentRow);

private:
    Ui::ImageDialog *ui;
    RPGSystem *system;
    int mode;
    QString current;
    QString folder_name;


    bool visible_hue = false;
    bool visible_options = false;
    bool visible_fog_options = false;
};

#endif // IMAGEDIALOG_H
