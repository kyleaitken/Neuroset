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

/*****************************************************************************
 * Controller [Controller Object]
 *
 *  - Controller thread handles operations of high level device event flow and
 *    interaction with realtime viewer object MainWindow
 *  - directly manages electrode concurrency events and discrete electrode
 *    operation
 *
 *    Controller is a member of MainWindow
 *      * thread execution in MainWindow Constructor
 *          via QThread function ' controller->moveToThread( QThread* ) '
 *
 *      * thread exit in MainWindow Deconstructor
 *
 *****************************************************************************/


class Controller : public QObject
{
    Q_OBJECT

public:
    Controller(QObject *parent = nullptr);
    ~Controller();

    // Public Member Functions
    bool electrodesConnected();
    void setPatientState(const QString& newState);
    bool isSessionActive();
    bool isSessionPaused();
    bool getElectrodesAreConnected();
    void setElectrodeContactRestored();

private:
    // Private Member Variables

    // attributes
    int numElectrodes = ELECTRODE_COUNT;       // hard coded number of electrodes (set to 7)
    QDateTime customDateTime;    // Manages internal device date set when user specifies a date/time on the device
    QDateTime referenceDateTime; // Manages internal device time initialized when user sets a custom date/time for time stamp calculations
    QTimer *sessionTimer;
    int remainingTime;

    // state
    mutable QMutex mutex;
    bool paused = false;
    bool electrodesHaveContact = false;
    bool sessionActive = false;
    PatientState currentPatientState = PatientState::Resting;

    // containers
    FileManager fileManager;                            // manages database related events
    QVector<Electrode *> electrodes;                    // container of pointers to electrodes
    QVector<QThread *> electrodeThreads;                // container of pointers to electrode threads - electrodes passed to threads via moveToThread()
    std::set<int> electrodesFinishedInitialBaseline;    // set to keep track of what electrodes finished their initial assigned task
    std::set<int> electrodesFinishedFinalBaseline;      // set to keep track of what electrodes finished their final assigned task
    std::set<int> electrodesFinishedTreatment;          // set to keep track of what electrodes finished their full assigned task

    // Private Member Functions
    void setupElectrodes();                             // CRITICAL CONTROLLER FUNCTION : initializes and executes all thread functions in device
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
    void pauseElectrodes();                         // device pause event signal
    void resumeSession();                           // device resume (start) event signal
    void stopElectrodes();                          // device stop event (gracefull exit accross flexible current state)
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
