#ifndef SOURCEDATA_H
#define SOURCEDATA_H

#include "defs.h"
#include <QtGlobal>
#include <QTime>
#include <QVector>
#include <QRandomGenerator>

// range of freq for delta
#define DELTA_LOW_HZ 1
#define DELTA_HIGH_HZ 4

// range of freq for alpha
#define ALPHA_LOW_HZ 8
#define ALPHA_HIGH_HZ 13

// range of freq for beta
#define BETA_LOW_HZ 13
#define BETA_HIGH_HZ 30

// range of freq for gamma
#define GAMMA_LOW_HZ 30
#define GAMMA_HIGH_HZ 100

// amplitude scaling for dominant/non-dominant bands
#define DOMINANT_AMP_LOW 1.5
#define DOMINANT_AMP_HIGH 2
#define NON_DOM_AMP_LOW 0.2
#define NON_DOM_AMP_HIGH 0.5


class SourceData
{
public:
    SourceData();

    QVector<EEGSourceData> getSourceData(PatientState state);
    double generateDominantAmplitude();
    double generateNonDominantAmplitude();
};

#endif // SOURCEDATA_H