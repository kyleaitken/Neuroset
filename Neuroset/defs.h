#ifndef DEFS_H
#define DEFS_H

#include <QVector>
#include <QString>
#include <QDebug>
#include <QMap>

using namespace std;

constexpr float TREATMENT_TIME_SECONDS = 45;            // total treatment time constant
extern const QVector<QString> eegSiteNames;             // eeg site names are constant QStrings
extern const QMap<QString, int> electrodeSiteNameToNum; // eeg site names to constants helper mapping container

#define ELECTRODE_COUNT 7         // number of electrodes for device
#define FULL_BATTERY_LEVEL 100 // full battery level for device to be initialized with
#define EMPTY_BATTERY_LEVEL 0 // empty battery level

// Device display screen type stack frame int constants
#define MENU_SCREEN 0               // device screen currently displays USER NEVIGATING MAIN MENU
#define TREATMENT_SCREEN 1          // device screen currently displays USER IN ACTIVE TREATMENT TIMER + BATTERY
#define WARNING_MESSAGE_SCREEN 2    // device screen currently displays DEVICE WARNING EVENT
#define SET_DATETIME_SCREEN 3       // device screen currently displays USER SETTING DATA + TIME
#define SESSION_LOG_SCREEN 4        // device screen currently displays USER VIEWING SESSION LOGS

// the following is int constants used for the brain wave generation.
#define DELTA 0        // code to distinguish wave types
#define THETA 1        // code to distinguish wave types
#define ALPHA 2        // code to distinguish wave types
#define BETA 3         // code to distinguish wave types
#define GAMMA 4        // code to distinguish wave types

// enumeration type for simulator to indicate to the source entity (sudo patient) what type of wave to produce
enum class PatientState {
    Sleeping,       // patient is sleeping = dominant DELTA Wave
    Resting,        // patient is resting = dominant THETA Wave
    ActiveTask,     // patient is undergoing moderate cognitive workload = dominant ALPHA Wave
    StressfulTask   // patient is undergoing high cognitive workload = dominant BETA Wave
};

// container struct to pass between classes the source data for EEG readings
struct EEGSourceData {
    double amplitude;   // amplitude of source wave
    int frequency;      // frequency of source wave
    double offset;      // offset of source wave
};

// container struct to hold the y and x sample plot data to pass to the QCustomPlot to display on the graph
struct Wave
{
    QVector<double> yPlot,xPlot;
};

#endif
