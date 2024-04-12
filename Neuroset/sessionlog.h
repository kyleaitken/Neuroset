#ifndef SESSIONLOG_H
#define SESSIONLOG_H

#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QVector>
#include "defs.h"
#include "frequencydata.h"

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
    QDateTime sessionDateTime; // QT object storing date and time session was completed
    QVector<FrequencyData> frequencyData;  // container object vector to store before and after freqeuncy data for each electrode
};

#endif
