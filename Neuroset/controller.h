#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QThread>
#include <QDateTime>
#include <QTimer>
#include <electrode.h>
#include <set>
#include <QMutex>
#include <QMutexLocker>
#include <defs.h>
#include <iostream>
#include "sessionlog.h"
#include "filemanager.h"

class Controller : public QObject
{
    Q_OBJECT

public:
    Controller(QObject *parent = nullptr);
    ~Controller();
    bool electrodesConnected();
    void setPatientState(const QString& newState);
    bool isSessionActive();
    bool isSessionPaused();
    bool getElectrodesAreConnected();
    void setElectrodeContactRestored();

private:
    // attributes
    int numElectrodes = 7;
    QDateTime customDateTime; // set when user specifies a date/time on the device
    QDateTime referenceDateTime; // initialize when user sets a custom date/time for time stamp calculations

    // state
    mutable QMutex mutex;
    bool paused = false;
    bool electrodesHaveContact = false;
    bool sessionActive = false;
    QTimer *sessionTimer;
    int remainingTime;
    PatientState currentPatientState = PatientState::Resting;

    // containees
    FileManager fileManager;
    QVector<Electrode *> electrodes;
    QVector<QThread *> electrodeThreads;
    std::set<int> electrodesFinishedInitialBaseline;
    std::set<int> electrodesFinishedFinalBaseline;
    std::set<int> electrodesFinishedTreatment;

    // methods
    void setupElectrodes();
    bool checkInitialBaselineFinished();
    bool checkFinalBaselineFinished();
    void startIndividualElectrodeTreatment(int electrodeNum);
    void recordSession();
    void resetState();

public slots:
    // Session control
    void startNewSession(); // starts a new treatment session
    void pauseSession();
    void resumeTreatmentSession();
    void stopSession();

    // Electrode session progress
    void setElectrodeFinishedInitialBaseline(int electrodeNum);
    void setElectrodeFinishedFinalBaseline(int electrodeNum);
    void setElectrodeFinishedTreatment(int electrodeNum);

    // Electrode contact
    void setElectrodeContactLost();
    void setElectrodeContactSecured();

    // Session logging
    void getSessionLogData(const QString &sessionName);
    void getPreviousSessionDatesForPC();
    void getPreviousSessionDatesForDevice();

    void updateTimeAndDate(QDateTime customDateTime, QDateTime referenceDateTime);
    void updateSessionTimerAndProgress();
    void slotGetElectrodeEEGWave(const QString& eName);

signals:
    void startElectrodeInitialBaseline(); // tells all electrodes to get their initial baseline
    void startElectrodeFinalBaseline();   // tells all electrodes to get their final baseline
    void startElectrodeTreatment(int electrodeNum);
    void pauseElectrodes();
    void resumeSession();
    void stopElectrodes();
    void powerStateChanged(bool newState);
    void updateTimerAndProgressDisplay(const QString& timer, int progressPercentage);
    void sessionDatesRetrieved(QStringList sessionDates);
    void sessionDatesRetrievedForDevice(QStringList sessionDates);
    void sessionLogDataRetrieved(QStringList sessionLogData);
    void signalDisplayElectrodeWave(const Wave& wave);
    void signalTreatmentSessionComplete();
    void signalUpdatedPatientState(PatientState newState);

};

#endif // CONTROLLER_H
