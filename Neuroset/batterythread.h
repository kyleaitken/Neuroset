#ifndef BATTERYTHREAD_H
#define BATTERYTHREAD_H
#include "battery.h"
#include <QThread>
class BatteryThread : public QThread
{
    Q_OBJECT
public:
    explicit BatteryThread(Battery *battery, QObject *parent = nullptr);
    void run();
    Battery *battery;
signals:
    void tellMainWindowBatteryPercentage(int curBattery);
};

#endif // BATTERYTHREAD_H
