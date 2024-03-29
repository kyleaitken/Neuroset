#ifndef FREQUENCYDATA_H
#define FREQUENCYDATA_H

#include <QString>

class FrequencyData {
public:
    FrequencyData(double before = 0, double after = 0, const QString& electrodeSiteName = QString());

    double getBefore() const;
    void setBefore(double value);

    double getAfter() const;
    void setAfter(double value);

    QString getElectrodeSiteName() const;
    void setElectrodeSiteName(const QString &value);

private:
    double before;
    double after;
    QString electrodeSiteName;
};

#endif // FREQUENCYDATA_H
