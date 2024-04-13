#ifndef FREQUENCYDATA_H
#define FREQUENCYDATA_H

#include <QString>

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

class FrequencyData {
public:
    FrequencyData(double before = 0, double after = 0, const QString& electrodeSiteName = QString());

    // getters
    double getBefore() const;
    double getAfter() const;
    QString getElectrodeSiteName() const;

   // setters
    void setBefore(double value);
    void setAfter(double value);
    void setElectrodeSiteName(const QString &value);
    void reset();

private:
    double before;
    double after;
    QString electrodeSiteName;
};

#endif // FREQUENCYDATA_H
