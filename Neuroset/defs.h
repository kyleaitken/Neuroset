#ifndef DEFS_H
#define DEFS_H

#include <QVector>
#include <QString>
#include <QDebug>
#include <QMap>

constexpr float TREATMENT_TIME_SECONDS = 50;
extern const QVector<QString> eegSiteNames;
extern const QMap<QString, int> electrodeSiteNameToNum;

using namespace std;

// the following is used for the brain wave generation. it is global to be used in main window and the patient entity Source
#define DELTA 0        // code to distinguish wave types
#define THETA 1        // code to distinguish wave types
#define ALPHA 2        // code to distinguish wave types
#define BETA 3         // code to distinguish wave types
#define GAMMA 4        // code to distinguish wave types
#define SAMPLE_TIME 65 // time the wave is sampled for -> used for display as plot cuts short before display max x value eg 10 20 30 40 50 -> 65 ensures 60 shows

#define SAMPLE_RATE 0.6                         // [Hz] rate at which y plot data is taken, eg x is taken every 0.6 seconds
#define DATA_START 0.0                          // shift the data to start at a different x value (issues with sine waves approaching 0)
#define DATA_POINTS (SAMPLE_TIME / SAMPLE_RATE) // number of plot samples
#define TREATMENT_RESPONSE_FACTOR 0.5           // [Hz] the max value in which the offset in patient producing brain waves MAY change based on each treatment

enum class PatientState {
    Sleeping,
    Resting,
    ActiveTask,
    StressfulTask
};

struct EEGSourceData {
    double amplitude;
    int frequency;
    double offset;
};

struct Wave
{
    QVector<double> yPlot,xPlot;
};

#endif
