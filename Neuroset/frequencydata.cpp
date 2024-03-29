#include "frequencydata.h"

FrequencyData::FrequencyData(double before, double after, const QString &electrodeSiteName)
    : before(before), after(after), electrodeSiteName(electrodeSiteName) {}

double FrequencyData::getBefore() const {
    return before;
}

void FrequencyData::setBefore(double value) {
    before = value;
}

double FrequencyData::getAfter() const {
    return after;
}

void FrequencyData::setAfter(double value) {
    after = value;
}

QString FrequencyData::getElectrodeSiteName() const {
    return electrodeSiteName;
}

void FrequencyData::setElectrodeSiteName(const QString &value) {
    electrodeSiteName = value;
}
