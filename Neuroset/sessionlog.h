#ifndef SESSIONLOG_H
#define SESSIONLOG_H

#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QVector>
#include "defs.h"
#include "frequencydata.h"

class SessionLog
{
public:
    SessionLog(QDateTime &sessionTime, QVector<FrequencyData> &freqData);

    // getters
    QDate getDate() const;
    QTime getTime() const;
    QDateTime getDateTime() const;
    QVector<FrequencyData> getFrequencyData() const;

private:
    QDateTime sessionDateTime; // could change this to string
    QVector<FrequencyData> frequencyData;
};

#endif
