
#include "batterythread.h"

BatteryThread::BatteryThread(Battery *battery, QObject *parent)
    : QThread(parent), battery(battery)
{
}

void BatteryThread::run()
{
    while (!isInterruptionRequested())
    {
        if (battery->isOn() && !battery->isCharging())
        {
            QThread::msleep(1500);
            battery->reduceBattery();
            if (battery->getBattery() % 5 == 0)
                qInfo() << "Device is ON and NOT charging. Battery is currently at " + QString::number(battery->getBattery());
            emit tellMainWindowBatteryPercentage(battery->getBattery());
        }
        else if (battery->isOn() && battery->isCharging())
        {
            QThread::sleep(5);
            battery->increaseBattery();
            if (battery->getBattery() % 5 == 0)
                qInfo() << "Device is ON and Charging. Battery is currently at " + QString::number(battery->getBattery());
            emit tellMainWindowBatteryPercentage(battery->getBattery());
        }
        else if (!battery->isOn() && battery->isCharging())
        {
            QThread::sleep(1);
            battery->increaseBattery();
            if (battery->getBattery() % 5 == 0)
                qInfo() << "Device is OFF and Charging. Battery is currently at " + QString::number(battery->getBattery());
            emit tellMainWindowBatteryPercentage(battery->getBattery());
        }
        else
        {
            QThread::sleep(1);
            // qInfo() << "Device is OFF and NOT charging. Battery is currently at " + QString::number(battery->getBattery());
        }
    }
}
