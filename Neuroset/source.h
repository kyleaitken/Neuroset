#ifndef SOURCE_H
#define SOURCE_H

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "defs.h"
#include <QObject>

//#define DELTA 0
#define D_HZ 0.45           // [Hz] avg Delta frequency in Hertz
#define D_AMP_SCALING 0.25 // adjusts amplitude of wave Delta

//#define THETA 1
#define T_HZ 5.0           // [Hz] Theta frequency in Hertz
#define T_AMP_SCALING 1.0 // adjusts amplitude of wave Theta

//#define ALPHA 2
#define A_HZ 10.0          // [Hz] Alpha frequency in Hertz
#define A_AMP_SCALING 1.0 // adjusts amplitude of wave Alpha

//#define BETA 3
#define B_HZ 20.0          // [Hz] Beta frequency in Hertz
#define B_AMP_SCALING 1.0 // adjusts amplitude of wave Beta

//#define GAMMA 4
#define G_HZ 40.0          // [Hz] Gamma frequency in Hertz
#define G_AMP_SCALING 1.0 // adjusts amplitude of wave Gamma


class Source
{
    private:
        double sourceDominantFrequency; // the frequency set based on current source neurological state
        bool sourceRecentlyStimulated;  // true if there was a successful stimulation event

        double generateNoise(double x, double amplitude);
        double generateDelta(double x, double hOff);          // Function to generate EEG data for Delta waves
        double generateTheta(double x, double hOff);          // Function to generate EEG data for Theta waves
        double generateAlpha(double x, double hOff);          // Function to generate EEG data for Alpha waves
        double generateBeta(double x, double hOff);           // Function to generate EEG data for Beta waves
        double generateGamma(double x, double hOff);          // Function to generate EEG data for Gamma waves

    public:
        Source();

        void setSourceDominantFrequency(double f);        // sets the frequency range the source will emit with this value as the mean
        QVector<double> brainWaveEmissionEvent(int waveCode);         // returns a single wave form using a list of sampled data points in microV / unit time (arbitrary unit currrently)
        void brainWaveStimulationEvent(QVector<double> ); // recieves a stumulation event and updates the current dominant frequency based on the dominant frequency passed in (emits 3Hz, recieves 8Hz, reemits 3.5Hz)
};

#endif // SOURCE_H
