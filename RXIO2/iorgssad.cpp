#include "iorgssad.h"
#include "rxexception.h"


IORGSSAD::IORGSSAD(QObject *parent)
{
    Q_UNUSED(parent);
}

IORGSSAD::IORGSSAD(QString path, QString output_dir)
{
    if (!output_dir.endsWith(QDir::separator()))
        output_dir.append(QDir::separator());

    this->file.setFileName(path);
    if (!file.open(QIODevice::ReadOnly))
        throw RXException("Could not open " + path + ": " + file.errorString());

    QByteArray header = file.read(8);
    if (header != QByteArray::fromHex("5247535341440001"))
        throw RXException("Invalid header");

    const QString output_root = QDir::cleanPath(QFileInfo(output_dir).absoluteFilePath()) + "/";

    while (!this->file.atEnd())
    {
        int name_length = this->read_and_decrypt_integer();
        if (name_length < 0 || name_length > 4096)
            throw RXException("Invalid file name length in archive");
        QString name = this->read_and_decrypt_string(name_length);
        name.replace("\\", "/"); // operating system independent
        int file_size = this->read_and_decrypt_integer();
        if (file_size < 0 || file_size > this->file.size() - this->file.pos())
            throw RXException("Invalid file size in archive: " + name);

        //do not allow the archive to write outside of the output directory (e.g. "../../file")
        QString output_path = QDir::cleanPath(output_root + name);
        if (QDir::isAbsolutePath(name) || !output_path.startsWith(output_root))
            throw RXException("Invalid file name in archive: " + name);

        QByteArray buffer = this->file.read(file_size);

        QDir().mkpath(QFileInfo(output_path).absolutePath());


        quint32 tempKey = (quint32)key;
        QByteArray key_array;
        key_array.resize(4);
        key_array[0] = ((tempKey >>  0) & 0xFF);
        key_array[1] = ((tempKey >>  8) & 0xFF);
        key_array[2] = ((tempKey >> 16) & 0xFF);
        key_array[3] = ((tempKey >> 24) & 0xFF);




        int j = 0;
        for (int i = 0; i <= buffer.length()-1; i++)
        {
            if (j == 4)
            {
                j = 0;
                tempKey *= 7;
                tempKey += 3;

                key_array[0] = ((tempKey >>  0) & 0xFF);
                key_array[1] = ((tempKey >>  8) & 0xFF);
                key_array[2] = ((tempKey >> 16) & 0xFF);
                key_array[3] = ((tempKey >> 24) & 0xFF);

            }

            buffer[i] = 0xFF & (buffer[i] ^ key_array[j]);

            j++;
        }

        QFile ofile(output_path);
        if (!ofile.open(QIODevice::WriteOnly))
            throw RXException("Could not write " + output_path + ": " + ofile.errorString());
        ofile.write(buffer);
        ofile.close();
    }

    file.close();

    QFile start_file(output_dir + "Game.rxproj");
    if (!start_file.open(QIODevice::WriteOnly))
        throw RXException("Could not write " + start_file.fileName() + ": " + start_file.errorString());
    start_file.write("RPGXP 1.05");
    start_file.close();
}

int IORGSSAD::read_and_decrypt_integer()
{
    int res = 0;
    res |= (this->read_one_byte() & 0xFF) ^ (key & 0xFF);
    res |= ((this->read_one_byte() & 0xFF) ^ ((key >>  8) & 0xFF) ) << 8;
    res |= ((this->read_one_byte() & 0xFF) ^ ((key >> 16) & 0xFF) ) << 16;
    res |= ((this->read_one_byte() & 0xFF) ^ ((key >> 24) & 0xFF) ) << 24;

    key *= 7;
    key += 3;

    return res;
}

QString IORGSSAD::read_and_decrypt_string(int length)
{
    QByteArray array = this->file.read(length);

    for (int i = 0; i <= array.length() - 1; i++)
    {
        array[i] = 0xFF & (array[i] ^ (key & 0xFF));
        key *= 7;
        key += 3;
    }

    return QString(array);
}

int IORGSSAD::read_one_byte()
{
    QByteArray one_byte = file.read(1);
    if (one_byte.length() != 1)
    {
        throw RXException("Unexpected EOF");
    }
    return 0xFF & (one_byte.at(0));
}
