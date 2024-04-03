#ifndef FREQUENCYDATA_H
#define FREQUENCYDATA_H

#include <QString>

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
