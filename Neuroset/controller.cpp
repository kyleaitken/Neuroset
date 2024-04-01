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

        // signals to tell electrodes to generate their initial and final baselines, start treatments, etc
        connect(this, &Controller::startElectrodeInitialBaseline, electrode, &Electrode::getInitialBaselineFrequency);
        connect(this, &Controller::startElectrodeFinalBaseline, electrode, &Electrode::getFinalBaselineFrequency);
        connect(this, &Controller::startElectrodeTreatment, electrode, &Electrode::startTreatmentListener);
        connect(this, &Controller::pauseElectrodes, electrode, &Electrode::handlePauseRequested);
        connect(this, &Controller::resumeSession, electrode, &Electrode::resume);

        // slots to keep track of electrodes that have finished their initial/final baselines
        connect(electrode, &Electrode::initialBaselineFinished, this, &Controller::setElectrodeFinishedInitialBaseline);
        connect(electrode, &Electrode::finalBaselineFinished, this, &Controller::setElectrodeFinishedFinalBaseline);
        connect(electrode, &Electrode::treatmentFinished, this, &Controller::setElectrodeFinishedTreatment);

        thread->start();
    }
}


void Controller::startNewSession(){
    if (paused) {
        qInfo() << "Restarting electrodes";
        emit resumeSession();
        paused = false;
    } else {
        emit startElectrodeInitialBaseline();
    }
}

// When the controller receives a signal from an electrode it's finished, adds it to the list of finished electrodes and
// then checks if they're all done. If they are, then it proceeds with individual electrode treatments
void Controller::setElectrodeFinishedInitialBaseline(int electrodeNum){
    QMutexLocker locker(&mutex);
    electrodesFinishedInitialBaseline.insert(electrodeNum);
    if (checkInitialBaselineFinished()){
        startIndividualElectrodeTreatment(0);
    }
}

// When the controller receives a signal from an electrode it's finished, adds it to the list of finished electrodes and
// then checks if they're all done. If they are, then it gathers data to send to the file manager
void Controller::setElectrodeFinishedFinalBaseline(int electrodeNum){
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
    QMutexLocker locker(&mutex);
    paused = true;
    emit pauseElectrodes();
}


void Controller::setElectrodeFinishedTreatment(int electrodeNum) {
    QMutexLocker locker(&mutex);
    electrodesFinishedTreatment.insert(electrodeNum);
    int numElectrodesFinished = static_cast<int>(electrodesFinishedTreatment.size());
    if (numElectrodesFinished == numElectrodes) {
        emit startElectrodeFinalBaseline();
    } else {
        startIndividualElectrodeTreatment(numElectrodesFinished);
    }
}

void Controller::newSession(){
    qDebug() << "New Session";
    //Handle new session
    //will emit to update MainWindow menu as appropriate
}
void Controller::sessionLog(){
    qDebug() << "Session Log";
    //Handle session log

    //will emit to update MainWindow menu as appropriate

}
void Controller::timeAndDate(){
    qDebug() << "Time and Date";
    //Handle time and date

    //will emit to update MainWindow menu as appropriate

}
void Controller::playButton(){
    qDebug() << "Play Button";

}
void Controller::pauseButton(){
    qDebug() << "Pause Button";

}
void Controller::stopButton(){
    qDebug() << "Stop Button";

}
