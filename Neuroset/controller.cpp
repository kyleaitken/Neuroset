#include "controller.h"
#include <iostream>

Controller::Controller(QObject *parent) : QObject(parent){
    setupElectrodes();
    sessionTimer = new QTimer(this);
    connect(sessionTimer, &QTimer::timeout, this, &Controller::updateSessionTimerAndProgress);
}

Controller::~Controller()
{
    delete sessionTimer;
    for (int i = numElectrodes-1; i >= 0; i--)
    {
//        Electrode *electrode = electrodes[i]; dont need to deallocate electrode ptr done in electrode threads by qt
        electrodes.pop_back();

        QThread *thread = electrodeThreads[i];
        electrodeThreads.pop_back();

        thread->quit();
        thread->wait();

        if(thread) delete thread;
    }
}


void Controller::setPatientState(const QString &newState) {
    if (newState == "Active Task") {
        currentPatientState = PatientState::ActiveTask;
    } else if (newState == "Sleeping") {
        currentPatientState = PatientState::Sleeping;
    } else if (newState == "Stressful Task") {
        currentPatientState = PatientState::StressfulTask;
    } else {
        currentPatientState = PatientState::Resting;
    }

    emit signalUpdatedPatientState(currentPatientState);
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
        connect(this, &Controller::startElectrodeInitialBaseline, electrode, &Electrode::startSession);
        connect(this, &Controller::startElectrodeFinalBaseline, electrode, &Electrode::getFinalBaselineFrequency);
        connect(this, &Controller::startElectrodeTreatment, electrode, &Electrode::startTreatmentListener);

        // pause/resume/stop
        connect(this, &Controller::pauseElectrodes, electrode, &Electrode::handlePauseRequested);
        connect(this, &Controller::resumeSession, electrode, &Electrode::resume);
        connect(this, &Controller::stopElectrodes, electrode, &Electrode::stop);

        // state updates
        connect(this, &Controller::signalUpdatedPatientState, electrode, &Electrode::slotUpdatePatientState);

        // slots to keep track of electrodes that have finished their initial/final baselines
        connect(electrode, &Electrode::initialBaselineFinished, this, &Controller::setElectrodeFinishedInitialBaseline);
        connect(electrode, &Electrode::finalBaselineFinished, this, &Controller::setElectrodeFinishedFinalBaseline);
        connect(electrode, &Electrode::treatmentFinished, this, &Controller::setElectrodeFinishedTreatment);

        thread->start();
    }
}


void Controller::startNewSession(){
    if (electrodesHaveContact) {
        sessionActive = true;
        remainingTime = TREATMENT_TIME_SECONDS;
        sessionTimer->start(1000);
        cout << "********************* STARTING TREATMENT SESSION *********************" << endl;
        emit startElectrodeInitialBaseline();
    }
}

void Controller::resumeTreatmentSession() {
    qInfo() << "Restarting electrodes";
    emit resumeSession();
    if (remainingTime > 0) {
        sessionTimer->start();
    }
    paused = false;
}

// When the controller receives a signal from an electrode it's finished, adds it to the list of finished electrodes and
// then checks if they're all done. If they are, then it proceeds with individual electrode treatments
void Controller::setElectrodeFinishedInitialBaseline(int electrodeNum){
    QMutexLocker locker(&mutex);
    electrodesFinishedInitialBaseline.insert(electrodeNum);
    if (checkInitialBaselineFinished()){
        qInfo() << "all electrodes finished initial baseline";
        startIndividualElectrodeTreatment(0);
    }
}

// When the controller receives a signal from an electrode it's finished, adds it to the list of finished electrodes and
// then checks if they're all done. If they are, then it gathers data to send to the file manager
void Controller::setElectrodeFinishedFinalBaseline(int electrodeNum){
    QMutexLocker locker(&mutex);
    electrodesFinishedFinalBaseline.insert(electrodeNum);
    if (checkFinalBaselineFinished()){
        qInfo() << "Electrodes have finished final baseline in thread ID: " << QThread::currentThreadId();
        cout << "********************* FINISHED TREATMENT SESSION *********************" << endl;
        recordSession();
    }
}


bool Controller::checkInitialBaselineFinished(){
    return static_cast<int>(electrodesFinishedInitialBaseline.size()) == numElectrodes;
}


bool Controller::checkFinalBaselineFinished(){
    return static_cast<int>(electrodesFinishedFinalBaseline.size()) == numElectrodes;
}

// Iterates through the electrodes and instructs them to perform their site specific eeg analysis and treatment
void Controller::startIndividualElectrodeTreatment(int electrodeNum) {
    emit startElectrodeTreatment(electrodeNum);
}


void Controller::recordSession() {
    QDateTime sessionDateTime;
    if (referenceDateTime.isNull()) {
        sessionDateTime = QDateTime::currentDateTime();
    } else {
        QDateTime currentDateTime = QDateTime::currentDateTime();
        qint64 elapsed = referenceDateTime.msecsTo(currentDateTime);
        sessionDateTime = customDateTime.addMSecs(elapsed);
    }

    QVector<FrequencyData> electrodeData;
    for (auto e : electrodes) {
        electrodeData.push_back(e->getFrequencyData());
    }
    SessionLog* session = new SessionLog(sessionDateTime, electrodeData);

    qInfo() << "Completed session at " << session->getDateTime();
    emit signalTreatmentSessionComplete();
    resetState();
    fileManager.addSessionLog(session);
}


void Controller::pauseSession() {
    QMutexLocker locker(&mutex);
    paused = true;
    sessionTimer->stop();
    emit pauseElectrodes();
}


void Controller::setElectrodeFinishedTreatment(int electrodeNum) {
    qInfo() << "setting electrode " << electrodeNum << " finished treatment";
    QMutexLocker locker(&mutex);
    electrodesFinishedTreatment.insert(electrodeNum);
    int numElectrodesFinished = static_cast<int>(electrodesFinishedTreatment.size());
    if (numElectrodesFinished == numElectrodes) {
        emit startElectrodeFinalBaseline();
    } else {
        startIndividualElectrodeTreatment(numElectrodesFinished);
    }
}

void Controller::updateTimeAndDate(QDateTime customDateTime, QDateTime referenceDateTime){
    this->customDateTime = customDateTime;
    this->referenceDateTime = referenceDateTime;
}


void Controller::stopSession(){
    emit stopElectrodes();
    sessionTimer->stop();
    resetState();
    cout << "********************* TREATMENT SESSION STOPPED *********************" << endl;
}

void Controller::resetState() {
    sessionActive = false;
    electrodesFinishedInitialBaseline.clear();
    electrodesFinishedFinalBaseline.clear();
    electrodesFinishedTreatment.clear();
}

void Controller::updateSessionTimerAndProgress() {
    if (remainingTime > 0) {
         remainingTime--;
         int minutes = remainingTime / 60;
         int seconds = remainingTime % 60;
         QString timeString = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
         float progressPercentage = (1.0f - static_cast<float>(remainingTime) / static_cast<float>(TREATMENT_TIME_SECONDS)) * 100;
         int percentageAsInt = static_cast<int>(progressPercentage);
         emit updateTimerAndProgressDisplay(timeString, percentageAsInt);
     } else {
         sessionTimer->stop();
     }
}

void Controller::getPreviousSessionDates() {
    QStringList sessionDates = fileManager.getSessionDates();
    emit sessionDatesRetrieved(sessionDates);
}

void Controller::getSessionLogData(const QString& sessionFileName) {
    QStringList sessionLogData = fileManager.getFileData(sessionFileName);
    emit sessionLogDataRetrieved(sessionLogData);
}

void Controller::slotGetElectrodeEEGWave(const QString& eName) {
    qInfo() << "Get eeg data from " << eName;
    int electrodeNum = electrodeSiteNameToNum.value(eName, -1);
    if (electrodeNum != -1) {
        qDebug() << "Site" << eName << "is electrode number" << electrodeNum;
        const Wave& electrodeWave = electrodes[electrodeNum]->getWaveData();
        if (electrodeWave.xPlot.length() > 0) {
            emit signalDisplayElectrodeWave(electrodeWave);
        }
    }

}

void Controller::setElectrodeContactLost(){
    if (sessionActive && electrodesHaveContact) {
        electrodesHaveContact = false;
        pauseSession();
    } else {
        electrodesHaveContact = false;
    }
}

void Controller::setElectrodeContactSecured(){
    if (isSessionPaused() && !electrodesHaveContact) {
        electrodesHaveContact = true;
        resumeTreatmentSession();
    } else {
        electrodesHaveContact = true;
    }
}

bool Controller::electrodesConnected() {
    return electrodesHaveContact;
}

bool Controller::isSessionActive() {
    return sessionActive;
}

bool Controller::isSessionPaused() {
    return paused;
}

bool Controller::getElectrodesAreConnected() {
    return electrodesHaveContact;
}

void Controller::setElectrodeContactRestored() {
    electrodesHaveContact = true;
}
