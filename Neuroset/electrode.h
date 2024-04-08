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
    FrequencyData freqData;
    mutable QMutex mutex;
    QWaitCondition pauseCondition;
    bool pauseRequested = false;
    bool stopRequested = false;
    PatientState patientState = PatientState::Resting;
    SourceData source;
    Wave EEGWaveData;


    // methods
    void startTreatment();
    double calculateDominantFrequency(const QVector<EEGSourceData>& EEGData);
    void generateWaveData(const QVector<EEGSourceData>& EEGData);

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
};

#endif // ELECTRODE_H
