#ifndef AUDIODIALOG_H
#define AUDIODIALOG_H


#include <QFileDialog>
#include <QWidget>
#include <QKeyEvent>
#include <QDir>
#include <QDirIterator>
#include <QSoundEffect>


namespace Ui {
class AudioDialog;
}

class RPGDB;

class AudioDialog : public QWidget
{
    Q_OBJECT

public:
    explicit AudioDialog(RPGDB *db, QString name, int volume, int pitch, int mode, QWidget *parent = nullptr);
    ~AudioDialog();
    enum {BGM, BGS, ME, SE};

    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->close();
        else if(e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }


    void update_audio_list();

signals:
    void ok_clicked(QString name);
    void ok_clicked(QString name, int volume, int pitch);

private slots:
    void on_button_ok_clicked();
    void on_button_cancel_clicked();
    void on_button_play_clicked();
    void on_button_stop_clicked();

    void on_list_doubleClicked(const QModelIndex &index);

    void on_button_import_clicked();

private:
    Ui::AudioDialog *ui;
    RPGDB *db;
    QString name;
    int mode = -1;
    QString music_type;
};

#endif // AUDIODIALOG_H
