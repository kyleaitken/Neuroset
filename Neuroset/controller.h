#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QThread>
#include <QDateTime>
#include <electrode.h>
#include <set>
#include <QMutex>
#include <QMutexLocker>
#include <defs.h>
#include "sessionlog.h"

class Controller : public QObject
{
    Q_OBJECT

    enum TreatmentStage {
        Idle,
        InitialBaseline,
        IndividualTreatment,
        FinalBaseline
    };

public:
    Controller(QObject *parent = nullptr);

private:
    // attributes
    int numElectrodes = 21;
    QDateTime customDateTime;
    QDateTime referenceDateTime;

    // state
    mutable QMutex mutex;
    TreatmentStage currentStage;

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

signals:
    void startElectrodeInitialBaseline(); // tells all electrodes to get their initial baseline
    void startElectrodeFinalBaseline();   // tells all electrodes to get their final baseline
    void startElectrodeTreatment(int electrodeNum);
};

#endif // CONTROLLER_H
