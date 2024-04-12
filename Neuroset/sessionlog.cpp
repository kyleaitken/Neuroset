#include "sessionlog.h"

/*****************************************************************************
 * SessionLog [Container Object]
 *
 *  - Container for the processed EEG data storing frequency values with a
 *    date and time of session completion
 *  - Data collected from electrode data in Controller and passed to
 *    FileManager to be written to file / stored to database
 *
 *    * Container does not exist as a Member of another object but instead is
 *      necessary to group time + date information and dominant frequency
 *      data of a eeg site electrode and pass it along to the database manager
 *
 *****************************************************************************/

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
