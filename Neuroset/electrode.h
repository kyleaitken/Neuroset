#ifndef ELECTRODE_H
#define ELECTRODE_H

#include <QObject>
#include <QThread>
#include <QDebug>

class Electrode : public QObject
{
    Q_OBJECT

public:
    Electrode(int electrodeNum);
    bool startTreatment();

private:
    int electrodeNum;
    int initialFreq;


signals:
    void initialBaselineFinished(int electrodeNum);
    void finalBaselineFinished(int electrodeNum);

public slots:
    void getBaselineFrequency();
    void getFinalBaselineFrequency();
};

#endif // ELECTRODE_H
