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

    void setupElectrodes();
    bool checkInitialBaselineFinished();
    void startIndividualElectrodeTreatment();

public slots:
    void startTreatment();
    void setElectrodeFinishedInitialBaseline(int electrodeNum);

signals:
    void startElectrodeInitialBaseline();
};

#endif // CONTROLLER_H
