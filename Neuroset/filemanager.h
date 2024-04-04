#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QStringList>
#include <QDateTime>
#include "frequencydata.h"

class FileManager
{
public:
    FileManager();

    void writeArrayToFile(const QString &relativeDirPath, const QString &fileName, const QStringList &array);

    QStringList readFileToArray(const QString &filePath);

    QString generateFileName();

    // void createFileWithData(const QDateTime &dateTime, const QVector<FrequencyData> &array)
};

#endif // FILEMANAGER_H
