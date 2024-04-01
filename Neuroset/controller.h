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

class Controller : public QObject
{
    Q_OBJECT

public:
    Controller(QObject *parent = nullptr);
private:
    // attributes
    int numElectrodes = 21;
    QDateTime customDateTime; // set when user specifies a date/time on the device
    QDateTime referenceDateTime; // initialize when user sets a custom date/time for time stamp calculations

    // state
    mutable QMutex mutex;
    bool paused = false;
    QTimer *sessionTimer;
    int remainingTime;

    // containees
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
    void updateSessionTimer();


signals:
    void startElectrodeInitialBaseline(); // tells all electrodes to get their initial baseline
    void startElectrodeFinalBaseline();   // tells all electrodes to get their final baseline
    void startElectrodeTreatment(int electrodeNum);
    void pauseElectrodes();
    void resumeSession();
    void stopElectrodes();
    void powerStateChanged(bool newState);
    void updateTimerView(const QString& timer);


};

#endif // CONTROLLER_H
