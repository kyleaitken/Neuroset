#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent){
    setupElectrodes();
}

// Initializes electrode threads, sets up signals/slots
void Controller::setupElectrodes(){
    for (int i = 0; i < numElectrodes; i++)
    {
        QThread *thread = new QThread(this);
        Electrode *electrode = new Electrode(i, eegSiteNames[i]);
        connect(thread, &QThread::finished, electrode, &QObject::deleteLater);

        electrode->moveToThread(thread);
        electrodeThreads.push_back(thread);
        electrodes.push_back(electrode);

        // signals to tell electrodes to generate their initial and final baselines
        connect(this, &Controller::startElectrodeInitialBaseline, electrode, &Electrode::getInitialBaselineFrequency);
        connect(this, &Controller::startElectrodeFinalBaseline, electrode, &Electrode::getFinalBaselineFrequency);

        // slots to keep track of electrodes that have finished their initial/final baselines
        connect(electrode, &Electrode::initialBaselineFinished, this, &Controller::setElectrodeFinishedInitialBaseline);
        connect(electrode, &Electrode::finalBaselineFinished, this, &Controller::setElectrodeFinishedFinalBaseline);

        thread->start();
    }
}


void Controller::startNewSession(){
    qInfo() << "Controller starting a new session, signalling to electrodes to get initial baseline ";
    emit startElectrodeInitialBaseline();
}

// When the controller receives a signal from an electrode it's finished, adds it to the list of finished electrodes and
// then checks if they're all done. If they are, then it proceeds with individual electrode treatments
void Controller::setElectrodeFinishedInitialBaseline(int electrodeNum){
    qInfo() << "Setting electrode " << electrodeNum << " to done initial baseline in controller thread: " << QThread::currentThreadId();
    QMutexLocker locker(&mutex);
    electrodesFinishedInitialBaseline.insert(electrodeNum);
    if (checkInitialBaselineFinished()){
        startIndividualElectrodeTreatment();
    }
}

// When the controller receives a signal from an electrode it's finished, adds it to the list of finished electrodes and
// then checks if they're all done. If they are, then it gathers data to send to the file manager
void Controller::setElectrodeFinishedFinalBaseline(int electrodeNum){
    qInfo() << "Setting electrode " << electrodeNum << " to done final baseline in controller thread: " << QThread::currentThreadId();
    QMutexLocker locker(&mutex);
    electrodesFinishedFinalBaseline.insert(electrodeNum);
    if (checkFinalBaselineFinished()){
        qInfo() << "Electrodes have finished final baseline";
        recordSession();
    }
}


bool Controller::checkInitialBaselineFinished(){
    return static_cast<int>(electrodesFinishedInitialBaseline.size()) == numElectrodes;
}


bool Controller::checkFinalBaselineFinished(){
    return static_cast<int>(electrodesFinishedInitialBaseline.size()) == numElectrodes;
}

// Iterates through the electrodes and instructs them to perform their site specific eeg analysis and treatment
void Controller::startIndividualElectrodeTreatment() {
    qInfo() << "Starting treatment";
    Electrode *e = nullptr;
    bool treatmentComplete = true;
    for (int i = 0; i < numElectrodes; i++)
    {
        e = electrodes[i];
        bool treatmentFinished = e->startTreatment();
        if (!treatmentFinished) {
            qInfo() << "Error completing treatment";
            treatmentComplete = false;
        }

        else {
            qInfo() << "Electrode " << i << " finished treatment";
        }
    }

    if (treatmentComplete) {
        emit startElectrodeFinalBaseline();
    } else {
        // This would be some kind of error handling
    }
}


void Controller::recordSession() {
    QDateTime sessionDateTime = QDateTime::currentDateTime(); // use current date/time for now until we handle custom user date/time
    QVector<FrequencyData> electrodeData;
    for (auto e : electrodes) {
        electrodeData.push_back(e->getFrequencyData());
    }
    SessionLog* session = new SessionLog(sessionDateTime, electrodeData);
    // send session log to file manager
}

