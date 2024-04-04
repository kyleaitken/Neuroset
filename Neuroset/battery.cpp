#include "battery.h"

battery::battery() {}

void battery::reduceBattery()
{
    this->curBattery -= 1;
}

void battery::setBattery(int battery)
{
    this->curBattery = battery;
}

int battery::getBattery()
{
    return this->curBattery;
}

bool battery::isCharging() const
{
    return this->charging;
}

bool battery::isOn() const
{
    return this->on;
}

void battery::setCharging(bool charging)
{
    this->charging = charging;
}

void battery::setOn(bool on)
{
    this->on = on;
}