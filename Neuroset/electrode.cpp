#include "electrode.h"

Electrode::Electrode(int electrodeNum, const QString &electrodeSiteName) : electrodeNum(electrodeNum)
{
    freqData.setElectrodeSiteName(electrodeSiteName);
}


void Electrode::getInitialBaselineFrequency() {
    qInfo() << "Electrode " << electrodeNum << " getting initial freq in thread: " << QThread::currentThreadId();

    // Simulate some work that can be broken up so we can check the pause requested state
    for (int i = 0; i < 5; i++) {
        QThread::msleep(2000);
        QCoreApplication::processEvents();
        if (pauseRequested) {
            qInfo() << "Electrode " << electrodeNum << " paused";
            while (pauseRequested) {
                QThread::msleep(100);
                QCoreApplication::processEvents();
            }
            qInfo() << "Electrode " << electrodeNum << " resumed";
        }
    }
    freqData.setBefore(30); // hard coded example
    emit initialBaselineFinished(electrodeNum);
}


void Electrode::getFinalBaselineFrequency(){
    qInfo() << "Electrode " << electrodeNum << " getting final freq in thread: " << QThread::currentThreadId();

    // Simulate some work that can be broken up so we can check the pause requested state
    for (int i = 0; i < 5; i++) {
        QThread::msleep(2000);
        QCoreApplication::processEvents();
        if (pauseRequested) {
            qInfo() << "Electrode " << electrodeNum << " paused";
            while (pauseRequested) {
                QThread::msleep(100);
                QCoreApplication::processEvents();
            }
            qInfo() << "Electrode " << electrodeNum << " resumed";
        }
    }
    freqData.setAfter(15); // hard coded example
    emit finalBaselineFinished(electrodeNum);
}


void Electrode::startTreatmentListener(int electrodeNum) {
    if (this->electrodeNum == electrodeNum) {
        startTreatment();
    }
}


bool Electrode::startTreatment()
{
    QThread::msleep(3000);
    emit treatmentFinished(electrodeNum);
    return true;
}


FrequencyData Electrode::getFrequencyData() const
{
    return freqData;
}

void Electrode::handlePauseRequested() {
    QMutexLocker locker(&pauseMutex);
    pauseRequested = true;
}


void Electrode::resume() {
    QMutexLocker locker(&pauseMutex);
    pauseRequested = false;
}
