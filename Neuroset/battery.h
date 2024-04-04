#ifndef BATTERY_H
#define BATTERY_H
#include "defs.h"
class Battery
{
public:
    Battery();
    int getBattery();
    void reduceBattery();
    void increaseBattery();
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
