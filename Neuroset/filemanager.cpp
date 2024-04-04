#include "filemanager.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QFileInfo>

FileManager::FileManager() {}
// QList<double> data = {1.2, 232.3, 34223.5}

// QStringList dataAsStringList;
// for (double value : data)
// {
//     dataAsString.append(QString::number(value));
// }

// void FileManager::createFileWithData(const QDateTime &dateTime, const QVector<FrequencyData> &array)
// {
//     QString relativeDirPath = "Data Output";
//     QString fileName = generateFileName(dateTime); // generates file name
//     QStringList data;
//     for (const FrequencyData &dataVals : array)
//     {
//         data << dataVals.toString();
//     }
//     writeArrayToFile(relativeDirPath, fileName, data);
// }

void FileManager::addSessionLog(SessionLog* log) {
    QString fileName = generateFileName(log->getDate());
    qInfo() << "filename: " << fileName;
    writeSessionDataToFile(fileName, log->getFrequencyData());
}

QString FileManager::generateFileName(const QDate &date) // const QDateTime &dateTime
{
    // check the files in data output directory and if theres one with matching datetime increment from 001 --> 002 etc.
    QString baseFileName = "Neuroset_Data_" + date.toString("yyyy-MM-dd");
    QString fileName = baseFileName + ".txt";

    QString dirPath = QDir::currentPath() + "\\Data Output";
    QDir directory(dirPath);

    int counter = 1;
    while (QFileInfo::exists(directory.absoluteFilePath(fileName)))
    {
        fileName = baseFileName + QString::number(counter) + ".txt";
        counter++;
    }
    return fileName;
}

void FileManager::writeSessionDataToFile(const QString &fileName, const QVector<FrequencyData> &freqData)
{
    qInfo() << fileName;
    QString homePath = QDir::homePath();
    // QString dirPath = QDir::cleanPath(homePath + QDir::separator() + relativeDirPath);
    QString dirPath = QDir::currentPath() + "/Data Output/";
    qInfo() << dirPath;
    qInfo() << dirPath + fileName;
    QDir dir(dirPath); // hi
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

QStringList FileManager::readFileToArray(const QString &filePath)
{
    QFile file(filePath);
    QStringList array;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return array;
    } // same deal with clutter

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine();
        array.append(line);
    }

    file.close();

    return array;
}

// int main()
// {
//     FileManager manager;
//     QStringList myArray = {"Line 1", "Line 2"};
//     QString relativeDirPath = "Data Output";
//     QString fileName = "Data1.txt";

//     manager.writeArrayToFile(relativeDirPath, fileName, myArray);
//     // QStringList readArray = manager.readFileToArray(QDir::currentPath() + "\\Data Output\\" + fileName);

//     // for (const QString &line : readArray)
//     // {
//     //     qDebug() << line;
//     // }

//     return 0;
// }
