#include "filemanager.h"
#include <iostream>

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

FileManager::FileManager() {}

void FileManager::addSessionLog(SessionLog *log)
{
    QString fileName = generateFileName(log->getDateTime());
    qInfo() << "filename: " << fileName;
    writeSessionDataToFile(fileName, log->getFrequencyData());
}

QString FileManager::generateFileName(const QDateTime &date) // const QDateTime &dateTime
{
    // check the files in data output directory and if theres one with matching datetime increment from 001 --> 002 etc.
    QString baseFileName = "Neuroset_Data_" + date.toString("yyyy-MM-dd_HH-mm-ss");
    QString fileName = baseFileName + ".txt";

    QString dirPath = QDir::currentPath() + "/Data Output";
    QDir directory(dirPath);

    cout << "Saving session file to database ";
    while (QFileInfo::exists(directory.absoluteFilePath(fileName)))
    {
        cout << "."; // previously file exists printout
        cout.flush();
        fileName = baseFileName + ".txt";
    }
    cout << endl;
    return fileName;
}

void FileManager::writeSessionDataToFile(const QString &fileName, const QVector<FrequencyData> &freqData)
{
    QString homePath = QDir::homePath();
    QString dirPath = QDir::currentPath() + "/Data Output/";

    QDir dir(dirPath);
    if (!dir.exists())
    {
        bool created = dir.mkpath(dirPath);
        if (!created)
        {
            qWarning() << "Could not create directory:" << dirPath;
            return;
        }
        qWarning() << dirPath;
    }

    QString filePath = QDir::cleanPath(dirPath + fileName);
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    { // might not need useless clutter but good for test
        return;
    }

    QTextStream out(&file);

    for (auto data : freqData)
    {
        out << data.getElectrodeSiteName() << ": " << data.getBefore() << ", " << data.getAfter() << "\n";
    }

    file.close();
}

QStringList FileManager::getFileData(const QString &partialFileName)
{
    QString filePath = QDir::currentPath() + "/" + relativeDirPath + "/Neuroset_Data_" + partialFileName + ".txt";

    QFile file(filePath);
    QStringList fileContent;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Could not open file for reading:" << filePath;
        return fileContent;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        fileContent.append(line);
    }

    file.close();
    return fileContent;
}

QStringList FileManager::getSessionDates()
{
    QString dirPath = QDir::currentPath() + "/Data Output";
    QDir directory(dirPath);

    QStringList dateStrings;
    QRegularExpression regex("Neuroset_Data_(\\d{4}-\\d{2}-\\d{2}_\\d{2}-\\d{2}-\\d{2}(?:_\\d+)?)\\.txt");

    QStringList files = directory.entryList(QStringList() << "*.txt", QDir::Files);
    foreach (const QString &file, files)
    {
        QRegularExpressionMatch match = regex.match(file);
        if (match.hasMatch())
        {
            QString dateString = match.captured(1);
            dateStrings << dateString;
        }
    }

    for (auto s : dateStrings)
    {
        qInfo() << "file date: " << s;
    }

    return dateStrings;
}
