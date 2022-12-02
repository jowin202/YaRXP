#include "audiodialog.h"
#include "ui_audiodialog.h"

#include "RXIO2/rpgdb.h"

AudioDialog::AudioDialog(RPGDB *db, QString name, int volume, int pitch, int mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudioDialog)
{
    ui->setupUi(this);

    this->db = db;
    this->mode = mode;

    this->name = name;

    this->ui->slider_volume->setValue(volume);
    this->ui->slider_pitch->setValue(pitch);


    switch(mode)
    {
    case BGM: music_type = "BGM"; break;
    case BGS: music_type = "BGS"; break;
    case ME: music_type = "ME"; break;
    case SE: music_type = "SE"; break;
    }
    this->setWindowTitle(music_type);

    this->update_audio_list();
}

AudioDialog::~AudioDialog()
{
    delete ui;
}

void AudioDialog::update_audio_list()
{
    this->ui->list->clear();
    this->ui->list->addItem("(None)");
    QDir music_dir(this->db->project_dir + "Audio" + QDir::separator() + music_type);


    QStringList entries = music_dir.entryList(QDir::Filter::NoFilter, QDir::SortFlag::Name);
    for (QString entry : entries)
    {
        if (entry == "." || entry == "..") continue;
        this->ui->list->addItem(entry);
        if (this->name == entry.chopped(4))
        {
            this->ui->list->setCurrentItem(this->ui->list->item(this->ui->list->count()-1));
        }
    }
}

void AudioDialog::on_button_ok_clicked()
{
    if (this->ui->list->currentItem() == 0)
        return; // do nothing

    if (this->ui->list->currentItem()!= 0)
        emit ok_clicked(this->ui->list->currentItem()->text().chopped(4),this->ui->slider_volume->value(), this->ui->slider_pitch->value());
    this->close();
}

void AudioDialog::on_button_cancel_clicked()
{
    this->close();
}

void AudioDialog::on_button_play_clicked()
{
    QSoundEffect effect;
    effect.setSource(QUrl::fromLocalFile(db->project_dir + "Audio" + QDir::separator() +  music_type + QDir::separator() + this->ui->list->currentItem()->text()));
    effect.play();
}

void AudioDialog::on_button_stop_clicked()
{

}

void AudioDialog::on_list_doubleClicked(const QModelIndex &index)
{
    if (index.row() >= 0)
        this->on_button_ok_clicked();
}

void AudioDialog::on_button_import_clicked()
{
    //TODO : Check this
    QStringList files = QFileDialog::getOpenFileNames(this, "Choose files", QDir::currentPath(), "");

    for (int i = 0; i < files.length(); i++)
    {
        if (QFile::exists(files.at(i)))
        {
            QFile::copy(files.at(i), db->project_dir + "Audio" + QDir::separator() + music_type + QDir::separator());
        }
    }

    this->update_audio_list();
}
