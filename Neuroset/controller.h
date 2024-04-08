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
#include "sessionlog.h"
#include "filemanager.h"

class Controller : public QObject
{
    Q_OBJECT

public:
    Controller(QObject *parent = nullptr);
private:
    // attributes
    int numElectrodes = 7;
    QDateTime customDateTime; // set when user specifies a date/time on the device
    QDateTime referenceDateTime; // initialize when user sets a custom date/time for time stamp calculations

    // state
    mutable QMutex mutex;
    bool paused = false;
    QTimer *sessionTimer;
    int remainingTime;
    PatientState currentState = PatientState::Resting;

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

public slots:
    void startNewSession(); // starts a new treatment session
    void setElectrodeFinishedInitialBaseline(int electrodeNum);
    void setElectrodeFinishedFinalBaseline(int electrodeNum);
    void setElectrodeFinishedTreatment(int electrodeNum);
    void pauseSession();
    void resumeTreatmentSession();
    void sessionLog();
    void timeAndDate();
    void stopSession();
    void updateSessionTimerAndProgress();
    void getPreviousSessionDates();
    void getSessionLogData(const QString &sessionName);
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
    void sessionLogDataRetrieved(QStringList sessionLogData);
    void signalDisplayElectrodeWave(const Wave& wave);

};

#endif // CONTROLLER_H
