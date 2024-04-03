#include "pc.h"
#include "defs.h"

/*
class PC : public QObject
{
    Q_OBJECT

    signals:
        void signalDisplayGraphData(QVector<double> yPlot);

    public:
        PC();

        void displayElectrodeEEG(QString electrodeSiteName);   // calls signal to emit data

    private:
        Source tempPatient;     // having a Source as a member of this entity is temperary to test graph functionality
*/

PC::PC(QObject* parent) : QObject(parent)
{}

void PC::displayElectrodeEEG(QString electrodeSiteName)
{
    cout << electrodeSiteName.toStdString() << " PC display request" << endl;
    QVector<double> temp = tempPatient.brainWaveEmissionEvent(DELTA);   // call to Source::brainWaveEmissionEvent(waveCode) -> only delta implemented rn
    emit signalDisplayGraphData(temp); // emit to mainWindow signal
}
