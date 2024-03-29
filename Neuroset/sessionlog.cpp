SessionLog::SessionLog(const QDate &date, const QTime &time, const QVector<FrequencyData> &freqData)
    : sessionDate(date), sessionTime(time), frequencyData(freqData) {}

QDate SessionLog::getDate() const
{
    return sessionDate;
}

QTime SessionLog::getTime() const
{
    return sessionTime;
}

QVector<FrequencyData> SessionLog::getFrequencyData() const
{
    return frequencyData;
}