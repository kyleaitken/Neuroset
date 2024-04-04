#ifndef BATTERY_H
#define BATTERY_H
#include "defs.h"
class battery
{
public:
    battery();
    int getBattery();
    void reduceBattery();
    void setBattery(int battery);

    // getters
    bool isCharging() const;
    bool isOn() const;

    // setters
    void setCharging(bool charging);
    void setOn(bool on);

private:
    int curBattery = 100;
    bool charging = false;
    bool on = false;
};

#endif // BATTERY_H
