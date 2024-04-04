#ifndef BATTERYTHREAD_H
#define BATTERYTHREAD_H

#include <QThread>
#include "battery.h"

class batterythread : public QThread
{
    Q_OBJECT

public:
    explicit batterythread(battery *battery, QObject *parent = nullptr);
    void run() override;

private:
    battery *battery;
};

#endif // BATTERYTHREAD_H
