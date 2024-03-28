#include "electrode.h"

Electrode::Electrode(int electrodeNum) : electrodeNum(electrodeNum) {
}

void Electrode::getBaselineFrequency() {
    qInfo() << "Electrode " << electrodeNum << " getting baseline freq in thread: " << QThread::currentThreadId();
    QThread::msleep(2000);
    emit initialBaselineFinished(electrodeNum);
}

void Electrode::getFinalBaselineFrequency() {
    qInfo() << "Electrode " << electrodeNum << " getting baseline freq in thread: " << QThread::currentThreadId();
    qInfo() << "intiial frequency for " << electrodeNum << " was " << initialFreq;
    QThread::msleep(2000);
    emit finalBaselineFinished(electrodeNum);
}

bool Electrode::startTreatment() {
    qInfo() << "Electrode " << electrodeNum << " starting treatment in thread: " << QThread::currentThreadId();
    QThread::msleep(3000);
    initialFreq = 30;
    return true;
}
