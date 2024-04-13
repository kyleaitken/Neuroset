#include "mainwindow.h"
#include "filemanager.h"
#include <QApplication>
#include <QMetaType>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<Wave>("Wave");
    qRegisterMetaType<PatientState>("PatientState");
    MainWindow w;
    w.show();

    return a.exec();
}
