#ifndef ELECTRODE_H
#define ELECTRODE_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QMap>
#include <QString>

class Electrode : public QObject
{
    Q_OBJECT

public:
    struct FrequencyData
    {
        QString electrodeSite;
        int initialFreq;
        int finalFreq;
    };

    Electrode(int electrodeNum, const QString &electrodeSiteName);
    bool startTreatment();
    FrequencyData getFrequencyData() const;

private:
    // members
    int electrodeNum;
    // FrequencyData freqData;

signals:
    void initialBaselineFinished(int electrodeNum); // signals to controller that this electrode has finished gathering its initial freq
    void finalBaselineFinished(int electrodeNum);   // signals to controller that this electrode has finished gathering its final freq

public slots:
    void getInitialBaselineFrequency();
    void getFinalBaselineFrequency();
};

#endif // ELECTRODE_H
