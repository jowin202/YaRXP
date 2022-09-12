#include "audiodialog.h"
#include "ui_audiodialog.h"

#include "RXIO/RXObjects/rpgsystem.h"

AudioDialog::AudioDialog(RPGSystem *system, RPGAudioFile *audiofile, int mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudioDialog)
{
    ui->setupUi(this);

    this->system = system;
    this->audiofile = audiofile;
    this->mode = mode;

    this->ui->slider_volume->setValue(this->audiofile->volume);
    this->ui->slider_pitch->setValue(this->audiofile->pitch);


    switch(mode)
    {
    case BGM: music_type = "BGM"; break;
    case BGS: music_type = "BGS"; break;
    case ME: music_type = "ME"; break;
    case SE: music_type = "SE"; break;
    }
    this->setWindowTitle(music_type);

    QDir music_dir(this->system->audio_dir + music_type);

    QString name = audiofile->name;


    this->ui->list->addItem("(None)");

    QDirIterator it(music_dir);
    while (it.hasNext()) {
        it.next();
        if (it.fileName() == "." || it.fileName() == "..")
            continue;

        this->ui->list->addItem(it.fileName());
        if (this->audiofile->name == it.fileName().chopped(4))
        {
            this->ui->list->setCurrentItem(this->ui->list->item(this->ui->list->count()-1));
        }
    }
}

AudioDialog::~AudioDialog()
{
    delete ui;
}

void AudioDialog::on_button_ok_clicked()
{
    if (this->ui->list->currentItem() == 0)
        return; // do nothing

    if (this->ui->list->currentRow() == 0)
        this->audiofile->name = RPGString("");
    else
        this->audiofile->name = this->ui->list->currentItem()->text().chopped(4);
    this->audiofile->volume = this->ui->slider_volume->value();
    this->audiofile->pitch = this->ui->slider_pitch->value();

    emit ok_clicked();
    this->close();
}

void AudioDialog::on_button_cancel_clicked()
{

}

void AudioDialog::on_button_play_clicked()
{
    QSound::play(system->audio_dir + music_type + QDir::separator() + this->ui->list->currentItem()->text());
}

void AudioDialog::on_button_stop_clicked()
{

}
