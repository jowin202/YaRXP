#ifndef AUDIODIALOG_H
#define AUDIODIALOG_H


#include <QWidget>
#include <QKeyEvent>
#include <QDir>
#include <QDirIterator>
#include <QSound>
#include "RXIO/RXObjects/rpgaudiofile.h"

namespace Ui {
class AudioDialog;
}

class RPGSystem;

class AudioDialog : public QWidget
{
    Q_OBJECT

public:
    explicit AudioDialog(RPGSystem *system, RPGAudioFile *audiofile, int mode, QWidget *parent = nullptr);
    ~AudioDialog();
    enum {BGM, BGS, ME, SE};

    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->close();
        else if(e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }

signals:
    void ok_clicked();

private slots:
    void on_button_ok_clicked();
    void on_button_cancel_clicked();
    void on_button_play_clicked();
    void on_button_stop_clicked();

private:
    Ui::AudioDialog *ui;
    RPGAudioFile *audiofile;
    RPGSystem *system;
    int mode = -1;
    QString music_type;
};

#endif // AUDIODIALOG_H
