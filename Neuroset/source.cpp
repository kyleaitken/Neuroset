#include "source.h"
#include <random>
#include <cmath>

Source::Source() : sourceDominantFrequency(D_HZ), sourceRecentlyStimulated(false)
{}


// Function to generate EEG data for Delta waves
double Source::generateDelta(double x, double hOff)
{
    // the following are all trial and error from viewing waves on Desmos
    double a1 = 1.4;
    double b1 = 1.4;

    double a2 = 0.4;
    double b2 = 1.0;
    double c2 = 2.3;

    double a3 = -1.1;

    double mainWave = a1*sin(b1*x-hOff) + a2*sin(b2*x+c2*x) + a3*sin(x);

    return D_HZ*mainWave;   // modify the height of the wave by Delta frequency average
}

// Function to generate EEG data for Theta waves
double Source::generateTheta(double x, double hOff)
{
    // the following are all trial and error from viewing waves on Desmos
    double a1 = 1.4;
    double b1 = 1.4;

    double a2 = 0.4;
    double b2 = 1.0;
    double c2 = 2.3;

    double a3 = -1.1;

    double mainWave = a1*sin(b1*x-hOff) + a2*sin(b2*x+c2*x) + a3*sin(x);

    return T_HZ*mainWave;   // modify the height of the wave by Theta frequency average
}

// Function to generate EEG data for Alpha waves
double Source::generateAlpha(double x, double hOff)
{
    // the following are all trial and error from viewing waves on Desmos
    double a1 = 1.4;
    double b1 = 1.4;

    double a2 = 0.4;
    double b2 = 1.0;
    double c2 = 2.3;

    double a3 = -1.1;

    double mainWave = a1*sin(b1*x-hOff) + a2*sin(b2*x+c2*x) + a3*sin(x);

    return A_HZ*mainWave;   // modify the height of the wave by Alpha frequency average
}


// Function to generate EEG data for Beta waves
double Source::generateBeta(double x, double hOff)
{
    // the following are all trial and error from viewing waves on Desmos
    double a1 = 1.4;
    double b1 = 1.4;

    double a2 = 0.4;
    double b2 = 1.0;
    double c2 = 2.3;

    double a3 = -1.1;

    double mainWave = a1*sin(b1*x-hOff) + a2*sin(b2*x+c2*x) + a3*sin(x);

    return B_HZ*mainWave;   // modify the height of the wave by Beta frequency average
}

// Function to generate EEG data for Gamma waves
double Source::generateGamma(double x, double hOff)
{
    // the following are all trial and error from viewing waves on Desmos
    double a1 = 1.4;
    double b1 = 1.4;

    double a2 = 0.4;
    double b2 = 1.0;
    double c2 = 2.3;

    double a3 = -1.1;

    double mainWave = a1*sin(b1*x-hOff) + a2*sin(b2*x+c2*x) + a3*sin(x);

    return G_HZ*mainWave;   // modify the height of the wave by Gamma frequency average
}

// sets the frequency range the source will emit with this value as the mean
void Source::setSourceDominantFrequency(double f) { sourceDominantFrequency = f; }


QVector<double> Source::brainWaveEmissionEvent(int waveCode)
{
    // commented out, option for variablility-> easier to include in main instead of invoking each call
    // seed random with system time
    //    srand(time(nullptr));

    int numPoints = DATA_POINTS;
    double hOff = ((double)rand() / RAND_MAX)*5.0;          // offset for generation event ensures random looking wave

    QVector<double> emissionWave;   // wave sample y data to be returned from calling this function

    double sourceOffSetMod = 0.0;   // offset after each treatment / multiple treatments to show device works

    // commented out, option for the next steps
//    // if there was treatment, modify eeg emmision from source to indicate treatment done (chance exists no indication from eeg)
//    if(!sourceRecentlyStimulated)
//    {
//        sourceRecentlyStimulated = true;
//    }
//    else
//    {
//        sourceOffSetMod = TREATMENT_RESPONSE_FACTOR * ((double)rand() / RAND_MAX); // create random feedback modification
//        if(sourceDominantFrequency>T_HZ) sourceOffSetMod*=(-1); // if the wave band is of a higher band make offset negative
//    }

    // emission wave is based on emmission event in simulator
    switch(waveCode)
    {
    case DELTA:
        // generate EEG data for DELTA wave
        for (int i = 0; i < numPoints; ++i)
        {
            double x = i*SAMPLE_RATE + DATA_START;    // DATA_START  points <20 may have an inconsistent look
            emissionWave.push_back(sourceOffSetMod + generateDelta(x, hOff));
        }
        break;
    case THETA:
        // generate EEG data for THETA wave
        for (int i = 0; i < numPoints; ++i)
        {
            double x = i*SAMPLE_RATE + DATA_START;    // DATA_START  points <20 may have an inconsistent look
            emissionWave.push_back(sourceOffSetMod + generateTheta(x, hOff));
        }
        break;
    case ALPHA:
        // generate EEG data for ALPHA wave
        for (int i = 0; i < numPoints; ++i)
        {
            double x = i*SAMPLE_RATE + DATA_START;    // DATA_START  points <20 may have an inconsistent look
            emissionWave.push_back(sourceOffSetMod + generateAlpha(x, hOff));
        }
        break;
    case BETA:
        // generate EEG data for BETA wave
        for (int i = 0; i < numPoints; ++i)
        {
            double x = i*SAMPLE_RATE + DATA_START;    // DATA_START  points <20 may have an inconsistent look
            emissionWave.push_back(sourceOffSetMod + generateBeta(x, hOff));
        }
        break;
    case GAMMA:
        // generate EEG data for GAMMA wave
        for (int i = 0; i < numPoints; ++i)
        {
            double x = i*SAMPLE_RATE + DATA_START;    // DATA_START  points <20 may have an inconsistent look
            emissionWave.push_back(sourceOffSetMod + generateGamma(x, hOff));
        }
        break;
    default:
        // no data generated
        cout << " BAD WAVE CODE SOURCE line: " << __LINE__ << endl;
        break;
    }
    return emissionWave;
}

void Source::brainWaveStimulationEvent(QVector<double> stimulationWave)
{
    cout << "\n SOURCE STIMULATED WITH WAVESIZE " << stimulationWave.size() << endl;
}

