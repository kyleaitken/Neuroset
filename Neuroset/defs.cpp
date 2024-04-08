#include "defs.h"

const QVector<QString> eegSiteNames = {
    "Fp1", "Fp2",
    "F7", "F3", "Fz", "F4", "F8",
    "T3", "C3", "Cz", "C4", "T4",
    "T5", "P3", "Pz", "P4", "T6",
    "O1", "O2",
    "A1", "A2"
};

const QMap<QString, int> electrodeSiteNameToNum = {
    {"Fp1", 0}, {"Fp2", 1},
    {"F7", 2}, {"F3", 3}, {"Fz", 4}, {"F4", 5}, {"F8", 6}
};
