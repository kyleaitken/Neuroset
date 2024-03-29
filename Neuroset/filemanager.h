#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QStringList>

class FileManager
{
public:
    FileManager();

    void writeArrayToFile(const QString &relativeDirPath, const QString &fileName, const QStringList &array);

    QStringList readFileToArray(const QString &filePath);
};

#endif // FILEMANAGER_H
