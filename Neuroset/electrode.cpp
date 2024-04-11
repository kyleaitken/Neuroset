#include "electrode.h"

Electrode::Electrode(int electrodeNum, const QString &electrodeSiteName) : electrodeNum(electrodeNum)
{
    electrodeName = electrodeSiteName;
    freqData.setElectrodeSiteName(electrodeSiteName);
}

void Electrode::startSession() {
    stopRequested = false;
    getInitialBaselineFrequency();
}


void Electrode::getInitialBaselineFrequency() {
    qInfo() << "Electrode " << electrodeName << " Calculating Dominant Frequency... ";
    QVector<EEGSourceData> EEGData = source.getSourceData(patientState);

    double dominantFrequency = calculateDominantFrequency(EEGData);
    double roundedFrequency = std::round(dominantFrequency * 10) / 10;

    freqData.setBefore(roundedFrequency);

    generateWaveData(EEGData);

}


void Electrode::generateWaveData(const QVector<EEGSourceData>& EEGData) {
    int dataPointsPerStep = 200;
    double samplingRate = 200.0;

    // RESET WAVE DATA BEFORE GENERATION
    EEGWaveData.xPlot.clear();
    EEGWaveData.yPlot.clear();

    // WAVE GENERATION
    for (int i = 0; i < 5; i++) {
        QThread::msleep(1200);
        QCoreApplication::processEvents();
        if (stopRequested) {
            freqData.reset();
            stopRequested = false;
            return;
        }
        if (pauseRequested) {
            while (pauseRequested) {
                QThread::msleep(100);
                QCoreApplication::processEvents();
            }
        }

        for (int j = 0; j < dataPointsPerStep; j++) {
            double time = (i * dataPointsPerStep + j) / samplingRate;
            EEGWaveData.xPlot.append(time);

            double yValue = 0.0;
            for (const EEGSourceData& data : EEGData) {
                yValue += data.amplitude * std::sin(2 * M_PI * data.frequency * time + data.offset);
            }
            EEGWaveData.yPlot.append(yValue);
        }
    }

    emit initialBaselineFinished(electrodeNum);
}



double Electrode::calculateDominantFrequency(const QVector<EEGSourceData> &EEGData) {
    double numerator = 0.0;
    double denominator = 0.0;

    for (const EEGSourceData& data : EEGData) {
        numerator += data.frequency * std::pow(data.amplitude, 2);
        denominator += std::pow(data.amplitude, 2);
    }

    return numerator / denominator;
}


void Electrode::getFinalBaselineFrequency(){
    for (int i = 0; i < 5; i++) {
        QThread::msleep(1200);
        QCoreApplication::processEvents();

        if (stopRequested) {
            freqData.reset();
            stopRequested = false;
            return;
        }

        if (pauseRequested) {
            while (pauseRequested) {
                QThread::msleep(100);
                QCoreApplication::processEvents();
            }
        }
    }

    // generate randome % between 0.7 and 0.85 to model a lowered dominant frequency from treatment
    double scaleFactor = QRandomGenerator::global()->generateDouble() * (0.85 - 0.7) + 0.7;
    double finalFrequency = std::round(freqData.getBefore() * scaleFactor * 10) / 10;

    freqData.setAfter(finalFrequency);
    emit finalBaselineFinished(electrodeNum);
}


void Electrode::startTreatmentListener(int electrodeNum) {
    if (this->electrodeNum == electrodeNum) {
        startTreatment();
    }
}


void Electrode::startTreatment()
{
    qInfo() << "ELECTRODE " << electrodeName << " EMITTING TREATMENT";
    int offset = 5;

    // Simulate some work that can be broken up so we can check the pause requested state
    for (int i = 0; i < 4; i++) {
        qInfo() << "Applying " << offset << "Hz Offset To Dominant Frequency: " << freqData.getBefore();
        QThread::msleep(1200);
        QCoreApplication::processEvents();

        if (stopRequested) {
            freqData.reset();
            stopRequested = false;
            return;
        }

        if (pauseRequested) {
            while (pauseRequested) {
                QThread::msleep(100);
                QCoreApplication::processEvents();
            }
        }
        offset += 5;
    }

    emit treatmentFinished(electrodeNum);
}


FrequencyData Electrode::getFrequencyData() const{
    return freqData;
}

const Wave& Electrode::getWaveData() const {
    return EEGWaveData;
}

void Electrode::handlePauseRequested() {
    QMutexLocker locker(&mutex);
    pauseRequested = true;
}


void Electrode::resume() {
    QMutexLocker locker(&mutex);
    pauseRequested = false;
}


void Electrode::stop() {
    QMutexLocker locker(&mutex);
    stopRequested = true;
    pauseRequested = false;
}

void Electrode::slotUpdatePatientState(PatientState newState) {
    patientState = newState;
}
