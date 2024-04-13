#include "frequencydata.h"

/*****************************************************************************
 * FrequencyData [Container Object]
 *
 *  - Stores treatment session data regarding processed frequencies only
 *
 *    FrequencyData is a member of Electrode
 *       * data has a before and after value aswell as a QString identifier
 *         for indicating which electrode site it originated
 *
 *       * all frequency data passed to FileManager via Controller and
 *         saved to database using FileManager::writeSessionDataToFile( ... )
 *
 *****************************************************************************/


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

void FrequencyData::reset() {
    before = 0;
    after = 0;
}
