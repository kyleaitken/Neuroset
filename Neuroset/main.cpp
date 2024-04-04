#include "mainwindow.h"
#include "filemanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // File Write + Read tests
//    FileManager manager;
//    QStringList myArray = {"Line 1", "Line 2"};
//    QString relativeDirPath = "Data Output";
//    QString fileName = "Data1.txt";

//    manager.writeArrayToFile(relativeDirPath, fileName, myArray);
    // QStringList readArray = manager.readFileToArray(QDir::currentPath() + "\\Data Output\\" + fileName);

    // for (const QString &line : readArray)
    // {
    //     qDebug() << line;
    // }

    return a.exec();
}
