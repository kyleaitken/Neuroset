#ifndef BATTERY_H
#define BATTERY_H
#include "defs.h"


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

class Battery
{
public:
    Battery();

    // Public Member Functions

    void reduceBattery();   // reduces battery level via device remaining on event
    void increaseBattery(); // increases battery level via charging event

    // getters
    int getBattery();
    bool isCharging() const;
    bool isOn() const;

    // setters
    void setBattery(int battery);
    void setCharging(bool charging);
    void setOn(bool on);

private:
    // Private Member Variables
    int curBattery = FULL_BATTERY_LEVEL;       // battery initialized with 100 power level
    bool charging = false;
    bool on = false;
};

#endif // BATTERY_H
