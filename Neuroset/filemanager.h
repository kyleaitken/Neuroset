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

/*****************************************************************************
 * FileManager [TextFile Database Manager Object]
 *
 *  - Handles Reading Writing events of session data using Text Files as a
 *    method of database 'hard' storage on program exit
 *
 *    FileManager is a member of Controller
 *       * Controller will pass the current session data to the FileManager
 *         instance when a full treatment session is complete
 *
 *       * File storage maintaned in a directory called 'Data Output' that
 *         will be created on first session file
 *
 *       * If the session is interupted, Controller will not invoke FileManager
 *         to store the incomplete session data to a file
 *
 *       * files stored with the following format:
 *         "Neuroset_Data_yyyy-MM-dd_HH-mm-ss"
 *
 *****************************************************************************/

class FileManager
{
public:
    FileManager();

    void addSessionLog(SessionLog* log);
    QStringList getSessionDates();
    QStringList getFileData(const QString& partialFileName);


private:
    QString relativeDirPath = "Data Output"; // directory where all files are stored

    QString generateFileName(const QDateTime &date);
    void writeSessionDataToFile(const QString &fileName, const QVector<FrequencyData> &freqData);


};

#endif // FILEMANAGER_H
