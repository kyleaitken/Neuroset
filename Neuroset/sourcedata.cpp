#include "sourcedata.h"

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

SourceData::SourceData()
{

}


QVector<EEGSourceData> SourceData::getSourceData(PatientState state) {
    QVector<EEGSourceData> data;


    // Generate 3 instances of each wave type
    for (int i = 0; i < 3; ++i) {
        data.append({generateNonDominantAmplitude(), QRandomGenerator::global()->bounded(DELTA_LOW_HZ, DELTA_HIGH_HZ + 1),
                     QRandomGenerator::global()->generateDouble() * 2 * M_PI});
        data.append({generateNonDominantAmplitude(), QRandomGenerator::global()->bounded(ALPHA_LOW_HZ, ALPHA_HIGH_HZ + 1),
                     QRandomGenerator::global()->generateDouble() * 2 * M_PI});
        data.append({generateNonDominantAmplitude(), QRandomGenerator::global()->bounded(BETA_LOW_HZ, BETA_HIGH_HZ + 1),
                     QRandomGenerator::global()->generateDouble() * 2 * M_PI});
        data.append({generateNonDominantAmplitude(), QRandomGenerator::global()->bounded(GAMMA_LOW_HZ, GAMMA_HIGH_HZ + 1),
                     QRandomGenerator::global()->generateDouble() * 2 * M_PI});
    }

    // Adjust the amplitude for the dominant wave type based on patient state
    switch (state) {
        case PatientState::Sleeping:
            // Delta dominant
            for (int i = 0; i < 3; ++i) {
                data[i * 4].amplitude = generateDominantAmplitude();
            }
            break;
        case PatientState::Resting:
            // Alpha dominant
            for (int i = 0; i < 3; ++i) {
                data[1 + i * 4].amplitude = generateDominantAmplitude();
            }
            break;
        case PatientState::ActiveTask:
            // Beta dominant
            for (int i = 0; i < 3; ++i) {
                data[2 + i * 4].amplitude = generateDominantAmplitude();
            }
            break;
        case PatientState::StressfulTask:
            // Gamma dominant
            for (int i = 0; i < 3; ++i) {
                data[3 + i * 4].amplitude = generateDominantAmplitude();
            }
            break;
    }

    return data;
}

double SourceData::generateDominantAmplitude() {
    return DOMINANT_AMP_LOW + QRandomGenerator::global()->generateDouble() * (DOMINANT_AMP_HIGH - DOMINANT_AMP_LOW);
}

double SourceData::generateNonDominantAmplitude() {
    return NON_DOM_AMP_LOW + QRandomGenerator::global()->generateDouble() * (NON_DOM_AMP_HIGH - NON_DOM_AMP_LOW);
}

