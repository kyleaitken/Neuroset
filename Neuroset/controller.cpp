#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent), currentStage(Idle){
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

        // signals to tell electrodes to generate their initial and final baselines, start treatments, etc
        connect(this, &Controller::startElectrodeInitialBaseline, electrode, &Electrode::getInitialBaselineFrequency);
        connect(this, &Controller::startElectrodeFinalBaseline, electrode, &Electrode::getFinalBaselineFrequency);
        connect(this, &Controller::startElectrodeTreatment, electrode, &Electrode::startTreatmentListener);

        // slots to keep track of electrodes that have finished their initial/final baselines
        connect(electrode, &Electrode::initialBaselineFinished, this, &Controller::setElectrodeFinishedInitialBaseline);
        connect(electrode, &Electrode::finalBaselineFinished, this, &Controller::setElectrodeFinishedFinalBaseline);
        connect(electrode, &Electrode::treatmentFinished, this, &Controller::setElectrodeFinishedTreatment);

        thread->start();
    }
}


void Controller::startNewSession(){
    qInfo() << "Controller starting a new session, signalling to electrodes to get initial baseline ";
    currentStage = InitialBaseline;
    emit startElectrodeInitialBaseline();
}

// When the controller receives a signal from an electrode it's finished, adds it to the list of finished electrodes and
// then checks if they're all done. If they are, then it proceeds with individual electrode treatments
void Controller::setElectrodeFinishedInitialBaseline(int electrodeNum){
    qInfo() << "Setting electrode " << electrodeNum << " to done initial baseline in controller thread: " << QThread::currentThreadId();
    QMutexLocker locker(&mutex);
    electrodesFinishedInitialBaseline.insert(electrodeNum);
    if (checkInitialBaselineFinished()){
        currentStage = IndividualTreatment;
        startIndividualElectrodeTreatment(0);
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
        currentStage = Idle;
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
void Controller::startIndividualElectrodeTreatment(int electrodeNum) {
    emit startElectrodeTreatment(electrodeNum);
}


void Controller::recordSession() {
    QDateTime sessionDateTime = QDateTime::currentDateTime(); // use current date/time for now until we handle custom user date/time
    QVector<FrequencyData> electrodeData;
    for (auto e : electrodes) {
        electrodeData.push_back(e->getFrequencyData());
    }
    SessionLog* session = new SessionLog(sessionDateTime, electrodeData);
    qInfo() << "Completed session at " << session->getDateTime();
    // send session log to file manager
}


void Controller::pauseSession() {
    qInfo() << "Add implementation for pausing session";
    // idea: sets a bool pauseState to true, electrodes check this boolean frequently in their computations if possible,
    // they wait if its true, then controller wakes threads again when user plays again
}


void Controller::setElectrodeFinishedTreatment(int electrodeNum) {
    QMutexLocker locker(&mutex);
    electrodesFinishedTreatment.insert(electrodeNum);
    int numElectrodesFinished = static_cast<int>(electrodesFinishedTreatment.size());
    if (numElectrodesFinished == numElectrodes) {
        currentStage = FinalBaseline;
        emit startElectrodeFinalBaseline();
    } else {
        startIndividualElectrodeTreatment(numElectrodesFinished);
    }
}
