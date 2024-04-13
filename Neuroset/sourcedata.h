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
#define NON_DOM_AMP_LOW 0.1
#define NON_DOM_AMP_HIGH 0.3

/*****************************************************************************
 * SourceData [Entity Object]
 *
 *  - Object to simulate a discrete EEG site producing unique wave data being
 *    passed from the patient to the electrode placed at that site
 *  - EEG brain wave data generated based on the PatientState enum set in the
 *    simulator
 *
 *    Member of Electrode
 *    *  Generates a random amplitude and frequency value in a hard capped
 *       range based on PatientState
 *
 *    *  Data is then fetched from its associated electrode site
 *
 *****************************************************************************/

class SourceData
{
public:
    SourceData();

    QVector<EEGSourceData> getSourceData(PatientState state);

private:
    double generateDominantAmplitude();
    double generateNonDominantAmplitude();
};

#endif // SOURCEDATA_H
