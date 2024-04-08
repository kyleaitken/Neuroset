#include "sessionlog.h"

SessionLog::SessionLog(QDateTime &sessionDateTime, QVector<FrequencyData> &freqData) : sessionDateTime(sessionDateTime), frequencyData(freqData) {}

QDate SessionLog::getDate() const
{
    return sessionDateTime.date();
}

QTime SessionLog::getTime() const
{
    return sessionDateTime.time();
}

QDateTime SessionLog::getDateTime() const
{
    return sessionDateTime;
}

QVector<FrequencyData> SessionLog::getFrequencyData() const
{
    return frequencyData;
}

// function here to send over data with this
// filemanager::createFileFromData(getDateTime(), frequencyData);
