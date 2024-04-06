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
#include "pc.h"
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
    void togglePowerStatus();
    void signalNewSession();
    void signalSessionLog();
    void signalTimeAndDate();
    void playButtonPressed();
    void pauseButtonPressed();
    void stopButtonPressed();
    void getPreviousSessionDates();
    void getSessionLogData(const QString &sessionFileName);

private slots:
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

    void updateUITimerAndProgress(const QString &timeString, int progressPercentage);
    void receiveBatteryPercentage(int curBattery);

    void slotDisplayGraphData(QVector<double> yPlot); // signal event emmitted from PC as a result of a ui display request
    void on_EEGSampleButton_clicked();
    void slotDisplaySessionDates(QStringList sessionDates);
    void slotDisplaySessionLogData(QStringList sessionLogData);

private:
    Ui::MainWindow *ui;
    QThread *controllerThread;
    Controller *controller;
    Battery *battery;
    BatteryThread *batterythread;
    PC *pc; // external device to test Neuroset device with display window for graphing EEG
    bool poweredOn = false;
    void turnDeviceScreenOff();
    void batteryDied();
};
#endif // MAINWINDOW_H
