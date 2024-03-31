#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <controller.h>
#include <QThread>
#include <QDebug>
#include <QThread>
#include <QListView>
#include <QStringListModel>

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

private slots:
    void on_upButton_clicked();

    void on_downButton_clicked();


    void on_powerButton_clicked();

    void on_selectButton_clicked();

    void on_playButton_clicked();

    void on_pauseButton_clicked();

    void on_stopButton_clicked();

private:
    Ui::MainWindow *ui;
    QThread *controllerThread;
    Controller *controller;
    bool poweredOn = false;

};
#endif // MAINWINDOW_H
