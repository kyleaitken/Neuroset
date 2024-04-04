#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QStringList>
#include <QDateTime>
#include "frequencydata.h"
#include "sessionlog.h"

class FileManager
{
public:
    FileManager();

    void addSessionLog(SessionLog* log);

    void writeSessionDataToFile(const QString &fileName, const QVector<FrequencyData> &freqData);

    QStringList readFileToArray(const QString &filePath);

    QString generateFileName(const QDate &date);

    // void createFileWithData(const QDateTime &dateTime, const QVector<FrequencyData> &array)

private:
    QString relativeDirPath = "Data Output";

};

#endif // FILEMANAGER_H
