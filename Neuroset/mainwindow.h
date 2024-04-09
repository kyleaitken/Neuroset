#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <controller.h>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QListView>
#include <QStringListModel>
#include "battery.h"
#include "batterythread.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void togglePower();

signals:
    // Session playout control
    void signalNewSession();
    void playButtonPressed();
    void pauseButtonPressed();
    void stopButtonPressed();

    // Electrode Contact
    void donHeadset();
    void electrodeContactLost();
    void electrodeContactRegained();

    void togglePowerStatus();
    void signalSessionLog();
    void signalTimeAndDate();

    // Session logging
    void getPreviousSessionDates();
    void getSessionLogData(const QString &sessionFileName);

    // EEG Graphing
    void signalGetElectrodeEEGWave(const QString& eName);

private slots:
    // Button handlers
    void on_chargeButton_clicked();
    void on_upButton_clicked();
    void on_downButton_clicked();
    void on_powerButton_clicked();
    void on_selectButton_clicked();
    void on_playButton_clicked();
    void on_pauseButton_clicked();
    void on_stopButton_clicked();
    void on_uploadButton_clicked();
    void onSessionDoubleClicked(const QModelIndex &index);
    void on_EEGSampleButton_clicked();
    void on_DonHeadset_clicked();
    void on_electrodeDisconnect_clicked();
    void on_electrodeReconnect_clicked();
    void on_patientStateComboBox_currentIndexChanged();

    // UI Updates
    void updateUITimerAndProgress(const QString &timeString, int progressPercentage);
    void receiveBatteryPercentage(int curBattery);
    void slotDisplayGraphData(const Wave& waveData); // signal event emmitted from PC as a result of a ui display request
    void slotDisplaySessionDates(QStringList sessionDates);
    void slotDisplaySessionLogData(QStringList sessionLogData);
    void slotTreatmentSessionComplete();
    void slotTreatmentApplicationStarted();
    void slotTreatmentApplicationFinished();

private:
    Ui::MainWindow *ui;
    QThread *controllerThread;
    Controller *controller;
    Battery *battery;
    BatteryThread *batterythread;
    bool poweredOn = false;
    int currScreenStack;
    QTimer *powerOffTimer;

    void turnDeviceScreenOff();
    void batteryDied();
    void displayMessage(const QString& message, int returnScreen);
};
#endif // MAINWINDOW_H
