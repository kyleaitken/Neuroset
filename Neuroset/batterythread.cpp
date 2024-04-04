
#include "batterythread.h"

batterythread::batterythread(battery *battery, QObject *parent)
    : QThread(parent), battery(battery)
{
}

void batterythread::run()
{
    while (!isInterruptionRequested())
    {
        QThread::sleep(1);
        battery->reduceBattery();
        qInfo() << "Battery is currently at " + battery->getBattery();
    }
}