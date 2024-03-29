#include "electrode.h"

Electrode::Electrode(int electrodeNum, const QString &electrodeSiteName) : electrodeNum(electrodeNum)
{
    freqData.electrodeSite = electrodeSiteName;
}

void Electrode::getInitialBaselineFrequency()
{
    qInfo() << "Electrode " << electrodeNum << " getting baseline freq in thread: " << QThread::currentThreadId();
    QThread::msleep(2000);
    freqData.initialFreq = 10;
    emit initialBaselineFinished(electrodeNum);
}

void Electrode::getFinalBaselineFrequency()
{
    qInfo() << "Electrode " << electrodeNum << " getting baseline freq in thread: " << QThread::currentThreadId();
    QThread::msleep(2000);
    freqData.finalFreq = 15;
    emit finalBaselineFinished(electrodeNum);
}

bool Electrode::startTreatment()
{
    qInfo() << "Electrode " << electrodeNum << " starting treatment in thread: " << QThread::currentThreadId();
    QThread::msleep(3000);
    return true;
}

// FrequencyData Electrode::getElectrodeData()
// {
//     return freqData;
// }
