#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    setupElectrodes();
}

void Controller::setupElectrodes()
{
    for (int i = 0; i < numElectrodes; i++)
    {
        QThread *thread = new QThread(this);
        Electrode *electrode = new Electrode(i, eegSiteNames[i]);
        connect(thread, &QThread::finished, electrode, &QObject::deleteLater);

        electrode->moveToThread(thread);
        electrodeThreads.push_back(thread);
        electrodes.push_back(electrode);

        // signals to tell electrodes to generate their initial and final baselines
        connect(this, &Controller::startElectrodeInitialBaseline, electrode, &Electrode::getInitialBaselineFrequency);
        connect(this, &Controller::startElectrodeFinalBaseline, electrode, &Electrode::getFinalBaselineFrequency);

        // slots to keep track of electrodes that have finished their initial/final baselines
        connect(electrode, &Electrode::initialBaselineFinished, this, &Controller::setElectrodeFinishedInitialBaseline);
        connect(electrode, &Electrode::finalBaselineFinished, this, &Controller::setElectrodeFinishedFinalBaseline);

        thread->start();
    }
}

void Controller::startTreatment()
{
    qInfo() << "Controller starting electrode initial baseline in thread: " << QThread::currentThreadId();
    emit startElectrodeInitialBaseline();
}

void Controller::setElectrodeFinishedInitialBaseline(int electrodeNum)
{
    qInfo() << "Setting electrode " << electrodeNum << " to done initial baseline in controller thread: " << QThread::currentThreadId();
    QMutexLocker locker(&mutex);
    electrodesFinishedInitialBaseline.insert(electrodeNum);
    if (checkInitialBaselineFinished())
    {
        startIndividualElectrodeTreatment();
    }
}

void Controller::setElectrodeFinishedFinalBaseline(int electrodeNum)
{
    qInfo() << "Setting electrode " << electrodeNum << " to done final baseline in controller thread: " << QThread::currentThreadId();
    QMutexLocker locker(&mutex);
    electrodesFinishedFinalBaseline.insert(electrodeNum);
    if (checkFinalBaselineFinished())
    {
        qInfo() << "Electrodes have finished final baseline";
    }
}

bool Controller::checkInitialBaselineFinished()
{
    return static_cast<int>(electrodesFinishedInitialBaseline.size()) == numElectrodes;
}

bool Controller::checkFinalBaselineFinished()
{
    return static_cast<int>(electrodesFinishedInitialBaseline.size()) == numElectrodes;
}

void Controller::startIndividualElectrodeTreatment()
{
    qInfo() << "Starting treatment";
    Electrode *e = nullptr;
    bool treatmentComplete = true;
    for (int i = 0; i < numElectrodes; i++)
    {
        e = electrodes[i];
        bool treatmentFinished = e->startTreatment();
        if (!treatmentFinished)
        {
            qInfo() << "Error completing treatment";
            treatmentComplete = false;
        }
        else
        {
            qInfo() << "Electrode " << i << " finished treatment";
        }
    }

    if (treatmentComplete)
    {
        emit startElectrodeFinalBaseline();
    }
}
