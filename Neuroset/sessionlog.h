#ifndef SESSIONLOG_H
#define SESSIONLOG_H

#include <QDate>
#include <QTime>
#include <QVector>
#include "defs.h"

class SessionLog
{
public:
    SessionLog(QDate &date, QTime &time, QVector<FrequencyData> &freqData);

    // getters
    QDate getDate() const;
    QTime getTime() const;
    QVector<FrequencyData> getFrequencyData() const;

private:
    QDate sessionDate; // could change this to string
    QTime sessionTime; // could change this to string
    QVector<FrequencyData> frequencyData;
};

#endif // SESSIONLOG_H