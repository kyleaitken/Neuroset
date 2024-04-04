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

    QString generateFileName(const QDateTime &dateTime);

    void createFileWithData(const QDateTime &dateTime, const QVector<FrequencyData> &array)
};

#endif // FILEMANAGER_H
