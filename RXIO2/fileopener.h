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
    QImage get_image();
    QString get_audio();
    QString get_existing_file();

private:
    QString path;
    QString name;
};

#endif // FILEOPENER_H
