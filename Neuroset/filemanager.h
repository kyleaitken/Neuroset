#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QStringList>

class FileManager
{
public:
    FileManager();

    void writeArrayToFile(const QString &relativeDirPath, const QStringList &array);

    QStringList readFileToArray(const QString &filePath);

    QString generateFileName();
};

#endif // FILEMANAGER_H
