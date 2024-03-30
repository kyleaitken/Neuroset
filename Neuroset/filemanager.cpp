#include "filemanager.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>

FileManager::FileManager() {}
// QList<double> data = {1.2, 232.3, 34223.5}

// QStringList dataAsStringList;
// for (double value : data)
// {
//     dataAsString.append(QString::number(value));
// }

// QString dirPath = ".";          // need to fig this out
// QString fileName = "data1.txt"; // this will be incremented and manipulated for new writes

void FileManager::writeArrayToFile(const QString &relativeDirPath, const QString &fileName, const QStringList &array)
{
    QString homePath = QDir::homePath();
    // QString dirPath = QDir::cleanPath(homePath + QDir::separator() + relativeDirPath);
    QString dirPath = QDir::currentPath() + "\\Data Output";
    // qInfo() << QDir::currentPath();
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

    QString filePath = QDir::cleanPath(dirPath + QDir::separator() + fileName);
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    { // might not need useless clutter but good for test
        return;
    }

    QTextStream out(&file);

    for (const QString &element : array)
    {
        out << element << "\n";
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

int main()
{
    FileManager manager;
    QStringList myArray = {"Line 1", "Line 2"};
    QString relativeDirPath = "Data Output";
    QString fileName = "Data1.txt";

    manager.writeArrayToFile(relativeDirPath, fileName, myArray);
    QStringList readArray = manager.readFileToArray(QDir::currentPath() + "\\Data Output\\" + fileName);

    for (const QString &line : readArray)
    {
        qDebug() << line;
    }

    return 0;
}
