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

class Electrode : public QObject
{
    Q_OBJECT

public:
    Electrode(int electrodeNum, const QString &electrodeSiteName);
    bool startTreatment();
    FrequencyData getFrequencyData() const;

private:
    // members
    int electrodeNum;
    FrequencyData freqData;
    mutable QMutex pauseMutex;
    QWaitCondition pauseCondition;
    bool pauseRequested = false;

signals:
    void initialBaselineFinished(int electrodeNum); // signals to controller that this electrode has finished gathering its initial freq
    void finalBaselineFinished(int electrodeNum);   // signals to controller that this electrode has finished gathering its final freq
    void treatmentFinished(int electrodeNum);

public slots:
    void getInitialBaselineFrequency();
    void getFinalBaselineFrequency();
    void startTreatmentListener(int electrodeNum);
    void handlePauseRequested();
    void resume();
};

#endif // ELECTRODE_H
