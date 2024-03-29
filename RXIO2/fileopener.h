#ifndef FILEOPENER_H
#define FILEOPENER_H

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QImage>

class FileOpener
{
public:
    FileOpener(QString path, QString name);

    QString get_existing_directory();
    QString get_existing_directory_or_create_it();
    QImage get_image();
    QString get_image_path();
    QString get_audio();
    QString get_existing_file();
private:
    QString path;
    QString name;
};

#endif // FILEOPENER_H
