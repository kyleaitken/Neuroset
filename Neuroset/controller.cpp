#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent){
    setupElectrodes();
    sessionTimer = new QTimer(this);
    connect(sessionTimer, &QTimer::timeout, this, &Controller::updateSessionTimer);
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

        // slots to keep track of electrodes that have finished their initial/final baselines
        connect(electrode, &Electrode::initialBaselineFinished, this, &Controller::setElectrodeFinishedInitialBaseline);
        connect(electrode, &Electrode::finalBaselineFinished, this, &Controller::setElectrodeFinishedFinalBaseline);
        connect(electrode, &Electrode::treatmentFinished, this, &Controller::setElectrodeFinishedTreatment);

        thread->start();
    }
}


void Controller::startNewSession(){
    remainingTime = 23 * 60;
    sessionTimer->start(1000);
    emit startElectrodeInitialBaseline();
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


void Controller::stopSession(){
    qDebug() << "Stop Button";
    emit stopElectrodes();

    sessionTimer->stop();

    electrodesFinishedInitialBaseline.clear();
    electrodesFinishedFinalBaseline.clear();
    electrodesFinishedTreatment.clear();
}

void Controller::updateSessionTimer() {
    if (remainingTime > 0) {
         remainingTime--;
         int minutes = remainingTime / 60;
         int seconds = remainingTime % 60;
         QString timeString = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
         emit updateTimerView(timeString);
     } else {
         sessionTimer->stop();
     }
}
