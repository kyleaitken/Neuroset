#ifndef PC_H
#define PC_H
#include <QObject>
#include "defs.h"
#include "source.h"

class PC : public QObject
{
    Q_OBJECT

    signals:
        void signalDisplayGraphData(QVector<double> yPlot);

    public:
        PC(QObject* parent = nullptr);

        void displayElectrodeEEG(QString electrodeSiteName);   // calls signal to emit data

    private:
        Source tempPatient;     // having a Source as a member of this entity is temperary to test graph functionality
};

#endif // PC_H
