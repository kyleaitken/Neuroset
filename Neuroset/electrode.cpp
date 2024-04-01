#include "electrode.h"

Electrode::Electrode(int electrodeNum, const QString &electrodeSiteName) : electrodeNum(electrodeNum)
{
    freqData.setElectrodeSiteName(electrodeSiteName);
}

void Electrode::startSession() {
    stopRequested = false;
    getInitialBaselineFrequency();
}


void Electrode::getInitialBaselineFrequency() {
    qInfo() << "Electrode " << electrodeNum << " getting initial freq in thread: " << QThread::currentThreadId();

    // Simulate some work that can be broken up so we can check the pause requested state
    for (int i = 0; i < 5; i++) {
        QThread::msleep(2000);
        QCoreApplication::processEvents();
        if (stopRequested) {
            qInfo() << "Electrode " << electrodeNum << " stop requested. Exiting initial baseline frequency gathering.";
            freqData.reset();
            stopRequested = false;
            return;
        }
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
        if (stopRequested) {
            qInfo() << "Electrode " << electrodeNum << " stop requested. Exiting final baseline frequency gathering.";
            freqData.reset();
            stopRequested = false;
            return;
        }
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


void Electrode::startTreatment()
{
    qInfo() << "Electrode " << electrodeNum << " performing treatment cycle";

    // Simulate some work that can be broken up so we can check the pause requested state
    for (int i = 0; i < 5; i++) {
        QThread::msleep(2000);
        QCoreApplication::processEvents();
        if (stopRequested) {
            qInfo() << "Electrode " << electrodeNum << " stop requested. Exiting treatment.";
            freqData.reset();
            stopRequested = false;
            return;
        }
        if (pauseRequested) {
            qInfo() << "Electrode " << electrodeNum << " paused";
            while (pauseRequested) {
                QThread::msleep(100);
                QCoreApplication::processEvents();
            }
            qInfo() << "Electrode " << electrodeNum << " resumed";
        }
    }

    emit treatmentFinished(electrodeNum);
}


FrequencyData Electrode::getFrequencyData() const
{
    return freqData;
}

void Electrode::handlePauseRequested() {
    QMutexLocker locker(&mutex);
    pauseRequested = true;
}


void Electrode::resume() {
    QMutexLocker locker(&mutex);
    pauseRequested = false;
}


void Electrode::stop() {
    qInfo() << "Stop requested in electrode thread " << QThread::currentThreadId();
    QMutexLocker locker(&mutex);
    stopRequested = true;
    pauseRequested = false;
}
