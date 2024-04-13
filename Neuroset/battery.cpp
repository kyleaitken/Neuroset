#include "battery.h"

/*****************************************************************************
 * Battery [Entity Object]
 *
 *  - entity handles battery level of device including depletion and charging
 *    events
 *  - implemented to update in real time via BatteryThread thread object
 *
 *    Battery is a member of BatteryThread
 *      * In BatteryThread it is handled in a thread function
 *
 *    Battery is a member of MainWindow
 *      * In MainWindow it is initialized and executed as a thread (as a
 *        BatteryThread)
 *
 *****************************************************************************/

Battery::Battery() {}

void Battery::reduceBattery()
{
    if (this->curBattery > EMPTY_BATTERY_LEVEL)
        this->curBattery -= 1;
}

void Battery::setBattery(int battery)
{
    this->curBattery = battery;
}

void Battery::increaseBattery()
{
    if (this->curBattery < FULL_BATTERY_LEVEL)
        this->curBattery += 1;
}

int Battery::getBattery()
{
    return this->curBattery;
}

bool Battery::isCharging() const
{
    return this->charging;
}

bool Battery::isOn() const
{
    return this->on;
}

void Battery::setCharging(bool charging)
{
    this->charging = charging;
}

void Battery::setOn(bool on)
{
    this->on = on;
}
