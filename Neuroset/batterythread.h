#ifndef BATTERYTHREAD_H
#define BATTERYTHREAD_H
#include "battery.h"
#include <QThread>

/*****************************************************************************
 * BatteryThread [Thread Object]
 *
 *  - Thread handles concurrent operations of Battery Entity
 *
 *    BatteryThread is a member of MainWindow
 *       * thread execution in MainWindow Constructor
 *          via thread function ' batterythread->start() '
 *
 *****************************************************************************/

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
