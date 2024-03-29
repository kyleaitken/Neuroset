#include "electrode.h"

Electrode::Electrode(int electrodeNum, const QString &electrodeSiteName) : electrodeNum(electrodeNum)
{
    freqData.setElectrodeSiteName(electrodeSiteName);
}

void Electrode::getInitialBaselineFrequency()
{
    qInfo() << "Electrode " << electrodeNum << " getting baseline freq in thread: " << QThread::currentThreadId();
    QThread::msleep(2000);
    freqData.setBefore(10);
    emit initialBaselineFinished(electrodeNum);
}

void Electrode::getFinalBaselineFrequency()
{
    qInfo() << "Electrode " << electrodeNum << " getting baseline freq in thread: " << QThread::currentThreadId();
    QThread::msleep(2000);
    freqData.setAfter(15); // hard coded example
    emit finalBaselineFinished(electrodeNum);
}

bool Electrode::startTreatment()
{
    qInfo() << "Electrode " << electrodeNum << " starting treatment in thread: " << QThread::currentThreadId();
    QThread::msleep(3000);
    return true;
}

FrequencyData Electrode::getFrequencyData() const
{
    return freqData;
}
