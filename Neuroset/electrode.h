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

/*****************************************************************************
 * Electrode [Entity Object]
 *
 *  - thread entity handles concurrent flow of EEG data between the controller
 *    and source
 *  - abstracted representation of a single electrode site that enables brain
 *    wave current flow between neuroset device and the patient, but also
 *    handles the device computational data processing and feedback events
 *    for the site
 *
 *    Electrode is a member of Controller
 *      * thread execution in Controller::setupElectrodes()
 *          via QThread function ' electrode->moveToThread( QThread* ) '
 *
 *      * thread exit in Controller Deconstructor
 *
 *****************************************************************************/

class Electrode : public QObject
{
    Q_OBJECT

public:
    Electrode(int electrodeNum, const QString &electrodeSiteName);

    // Public Member Functions
    FrequencyData getFrequencyData() const;
    const Wave& getWaveData() const;

private:
    // Private Member Variables

    // attributes
    int electrodeNum;       // eeg site device internal identity code
    QString electrodeName;  // eeg site = electrode name

    // states
    mutable QMutex mutex;         // thread safety mutex
    bool pauseRequested = false; // specific event thread safety flag
    bool stopRequested = false; // specific event thread safety flag
    PatientState patientState = PatientState::Resting; // QT simulation enum variable set from admin view in mainwindow

    // containers
    FrequencyData freqData;     // data regarding wave frequency being stored for pre and post processing event
    SourceData source;          // data passed from patient stored for pre and post processing event
    Wave EEGWaveData;           // current eeg wave data being updated and stored for pre and post processing event to this site

    // Private Member Functions
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
