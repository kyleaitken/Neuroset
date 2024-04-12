#ifndef ELECTRODE_H
#define ELECTRODE_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QMap>
#include <QString>
#include <QMutex>
#include <QWaitCondition>
#include <QCoreApplication>
#include <QMutexLocker>
#include "frequencydata.h"
#include "defs.h"
#include "sourcedata.h"
#include <cmath>
#include <iostream>


class Electrode : public QObject
{
    Q_OBJECT

public:
    Electrode(int electrodeNum, const QString &electrodeSiteName);
    FrequencyData getFrequencyData() const;
    const Wave& getWaveData() const;

private:
    // members
    int electrodeNum;
    QString electrodeName;
    mutable QMutex mutex;
    bool pauseRequested = false;
    bool stopRequested = false;
    PatientState patientState = PatientState::Resting;

    FrequencyData freqData;
    SourceData source;
    Wave EEGWaveData;


    // methods
    void startTreatment();
    double calculateDominantFrequency(const QVector<EEGSourceData>& EEGData);
    void generateWaveData(const QVector<QVector<EEGSourceData>>& EEGData);

signals:
    void initialBaselineFinished(int electrodeNum); // signals to controller that this electrode has finished gathering its initial freq
    void finalBaselineFinished(int electrodeNum);   // signals to controller that this electrode has finished gathering its final freq
    void treatmentFinished(int electrodeNum);

public slots:
    void startSession();
    void getInitialBaselineFrequency();
    void getFinalBaselineFrequency();
    void startTreatmentListener(int electrodeNum);
    void handlePauseRequested();
    void resume();
    void stop();
    void slotUpdatePatientState(PatientState newState);
};

#endif // ELECTRODE_H
