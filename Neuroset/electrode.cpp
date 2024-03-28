#include "electrode.h"

Electrode::Electrode(int electrodeNum) : electrodeNum(electrodeNum) {
}

void Electrode::getBaselineFrequency() {
    qInfo() << "Electrode " << electrodeNum << " getting baseline freq in thread: " << QThread::currentThreadId();
    QThread::msleep(2000);
    emit initialBaselineFinished(electrodeNum);
}

bool Electrode::startTreatment() {
    qInfo() << "Electrode " << electrodeNum << " starting treatment in thread: " << QThread::currentThreadId();
    QThread::msleep(3000);
    return true;
}
