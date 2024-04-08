#include "battery.h"

Battery::Battery() {}

void Battery::reduceBattery()
{
    if (this->curBattery > 0)
        this->curBattery -= 1;
}

void Battery::setBattery(int battery)
{
    this->curBattery = battery;
}

void Battery::increaseBattery()
{
    if (this->curBattery < 100)
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