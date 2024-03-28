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

signals:
    void initialBaselineFinished(int electrodeNum);

public slots:
    void getBaselineFrequency();
};

#endif // ELECTRODE_H
