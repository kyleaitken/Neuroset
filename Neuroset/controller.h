#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QThread>
#include <electrode.h>
#include <set>
#include <QMutex>
#include <QMutexLocker>

class Controller : public QObject
{
    Q_OBJECT

    mutable QMutex mutex;

public:
    Controller(QObject* parent = nullptr);

private:
    int numElectrodes = 3;
    QVector<Electrode*> electrodes;
    QVector<QThread*> electrodeThreads;
    std::set<int> electrodesFinishedInitialBaseline;
    std::set<int> electrodesFinishedFinalBaseline;


    void setupElectrodes();
    bool checkInitialBaselineFinished();
    bool checkFinalBaselineFinished();
    void startIndividualElectrodeTreatment();

public slots:
    void startTreatment();
    void setElectrodeFinishedInitialBaseline(int electrodeNum);
    void setElectrodeFinishedFinalBaseline(int electrodeNum);

signals:
    void startElectrodeInitialBaseline();
    void startElectrodeFinalBaseline();
};

#endif // CONTROLLER_H
