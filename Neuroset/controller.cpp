#include "controller.h"

Controller::Controller(QObject* parent): QObject(parent) {
    setupElectrodes();
}


void Controller::setupElectrodes() {
    for (int i = 0; i < numElectrodes; i++) {
        QThread* thread = new QThread(this);
        Electrode* electrode = new Electrode(i);
        connect(thread, &QThread::finished, electrode, &QObject::deleteLater);

        electrode->moveToThread(thread);

        electrodeThreads.push_back(thread);
        electrodes.push_back(electrode);

        connect(this, &Controller::startElectrodeInitialBaseline, electrode, &Electrode::getBaselineFrequency);
        connect(electrode, &Electrode::initialBaselineFinished, this, &Controller::setElectrodeFinishedInitialBaseline);


        thread->start();
    }
}


void Controller::startTreatment() {
    qInfo() << "Controller starting electrode initial baseline in thread: " << QThread::currentThreadId();
    emit startElectrodeInitialBaseline();
}


void Controller::setElectrodeFinishedInitialBaseline(int electrodeNum) {
    qInfo() << "Setting electrode " << electrodeNum << " to done initial baseline in controller thread: " <<QThread::currentThreadId();
    QMutexLocker locker(&mutex);
    electrodesFinishedInitialBaseline.insert(electrodeNum);
    if (checkInitialBaselineFinished()) {
        startIndividualElectrodeTreatment();
    }
}


bool Controller::checkInitialBaselineFinished() {
    return static_cast<int>(electrodesFinishedInitialBaseline.size()) == numElectrodes;
}


void Controller::startIndividualElectrodeTreatment() {
    qInfo() << "Starting treatment";
    Electrode* e = nullptr;
    for (int i = 0; i < numElectrodes; i++) {
        e = electrodes[i];
        bool treatmentFinished = e->startTreatment();
        if (!treatmentFinished) {
            qInfo() << "Error completing treatment";
        } else {
            qInfo() << "Electrode " << i << " finished treatment";
        }
    }

}

