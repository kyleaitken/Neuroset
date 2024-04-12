#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QRegularExpression>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include "frequencydata.h"
#include "sessionlog.h"


class FileManager
{
public:
    FileManager();

    void addSessionLog(SessionLog* log);
    QStringList getSessionDates();
    QStringList getFileData(const QString& partialFileName);


private:
    QString relativeDirPath = "Data Output";

    QString generateFileName(const QDateTime &date);
    void writeSessionDataToFile(const QString &fileName, const QVector<FrequencyData> &freqData);


};

#endif // FILEMANAGER_H
