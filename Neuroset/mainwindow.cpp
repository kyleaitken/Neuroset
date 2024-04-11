#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{

    powerOffTimer = new QTimer(this);
    powerOffTimer->setSingleShot(true);
    connect(powerOffTimer, &QTimer::timeout, this, &MainWindow::turnDeviceScreenOff);
    connect(powerOffTimer, &QTimer::timeout, this, &MainWindow::togglePower);

    // Setting up UI icons
    ui->setupUi(this);
    ui->powerButton->setIcon(QIcon(":/images/images/PowerOff.png"));
    ui->stopButton->setIcon(QIcon(":/images/images/stop.png"));
    ui->playButton->setIcon(QIcon(":/images/images/play.png"));
    ui->pauseButton->setIcon(QIcon(":/images/images/pause.png"));
    ui->upButton->setIcon(QIcon(":/images/images/up.png"));
    ui->downButton->setIcon(QIcon(":/images/images/down.png"));
    ui->menuButton->setIcon(QIcon(":/images/images/Menu.png"));
    QPixmap pixmap(":/images/images/4Battery.png");
    ui->battery->setPixmap(pixmap);

    controllerThread = new QThread(this);
    controller = new Controller();
    controller->moveToThread(controllerThread);

    this->battery = new Battery();
    this->batterythread = new BatteryThread(this->battery);
    this->batterythread->start();

    // Setting up Menu settings + styling
    ui->menuView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->menuView->setStyleSheet(R"(
        QListView {
            font-size: 14pt; /* Adjust the size as needed */
        }
        QListView::item {
            color: black; /* Text color for items */
            background-color: white; /* Background color for items */
        }
        QListView::item:selected {
            font-weight: bold; /* Make selected items bold */
            background-color: #E0E0E0; /* Background color for selected items */
        }
        QListView::item:selected:!active {
            background-color: #E0E0E0; /* Background color for selected items when not focused */
        }
    )");
    ui->menuView->setSelectionMode(QAbstractItemView::NoSelection);

    // Set up previous sessions list
    ui->prevSessionsList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->prevSessionsList->setSelectionBehavior(QAbstractItemView::SelectItems);
    connect(ui->prevSessionsList, &QListView::doubleClicked, this, [this](const QModelIndex &index) {
        this->onSessionDoubleClicked(index);
    });

    ui->upButton->setEnabled(false);
    ui->downButton->setEnabled(false);
    ui->playButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
    ui->selectButton->setEnabled(false);
    ui->electrodeDisconnect->setEnabled(false);
    ui->electrodeReconnect->setEnabled(false);


    // MainWindow Signals to Controller Slots
    connect(this, &MainWindow::signalNewSession, controller, &Controller::startNewSession);
    connect(this, &MainWindow::signalTimeAndDate, controller, &Controller::updateTimeAndDate);
    connect(this, &MainWindow::playButtonPressed, controller, &Controller::resumeTreatmentSession);
    connect(this, &MainWindow::pauseButtonPressed, controller, &Controller::pauseSession);
    connect(this, &MainWindow::stopButtonPressed, controller, &Controller::stopSession);
    connect(this, &MainWindow::getPreviousSessionDates, controller, &Controller::getPreviousSessionDates);
    connect(this, &MainWindow::getSessionLogData, controller, &Controller::getSessionLogData);
    connect(this, &MainWindow::signalGetElectrodeEEGWave, controller, &Controller::slotGetElectrodeEEGWave);

    // Electrode Contact
    connect(this, &MainWindow::electrodeContactLost, controller, &Controller::setElectrodeContactLost);
    connect(this, &MainWindow::electrodeContactRegained, controller, &Controller::setElectrodeContactSecured);

    // Battery Signals to MainWindow Slots
    connect(batterythread, &BatteryThread::tellMainWindowBatteryPercentage, this, &MainWindow::receiveBatteryPercentage);

    // Controller Signals to MainWindow Slots
    connect(controller, &Controller::updateTimerAndProgressDisplay, this, &MainWindow::updateUITimerAndProgress);
    connect(controller, &Controller::sessionDatesRetrieved, this, &MainWindow::slotDisplaySessionDates);
    connect(controller, &Controller::sessionLogDataRetrieved, this, &MainWindow::slotDisplaySessionLogData);
    connect(controller, &Controller::signalDisplayElectrodeWave, this, &MainWindow::slotDisplayGraphData);
    connect(controller, &Controller::signalTreatmentSessionComplete, this, &MainWindow::slotTreatmentSessionComplete);
    connect(controller, &Controller::startElectrodeTreatment, this, &MainWindow::slotTreatmentApplicationStarted);
    connect(controller, &Controller::startElectrodeFinalBaseline, this, &MainWindow::slotTreatmentApplicationFinished);

    controllerThread->start();
}

MainWindow::~MainWindow()
{
    batteryDied();  // included battery died to safely stop every event

    cout << " EXITING NEUROSET SIMULATION .... " << endl;

    powerOffTimer->stop();

    controllerThread->quit();
    controllerThread->wait();

    delete controllerThread;

    batterythread->requestInterruption();
    batterythread->wait();

    delete batterythread;

    delete powerOffTimer;

    delete controller;

    delete battery;

    delete ui;
}

void MainWindow::on_upButton_clicked()
{
    if (ui->screenStack->currentIndex() == TREATMENT_SCREEN) return;

    int currentRow = ui->menuView->currentIndex().row();
    int previousRow = currentRow > 0 ? currentRow - 1 : 0;
    QModelIndex newIndex = ui->menuView->model()->index(previousRow, 0);
    ui->menuView->setCurrentIndex(newIndex);
    ui->menuView->setFocus();
}

void MainWindow::on_downButton_clicked()
{
    if (ui->screenStack->currentIndex() == TREATMENT_SCREEN) return;

    int currentRow = ui->menuView->currentIndex().row();
    int rows = ui->menuView->model()->rowCount();
    int nextRow = currentRow < rows - 1 ? currentRow + 1 : rows - 1;
    QModelIndex newIndex = ui->menuView->model()->index(nextRow, 0);
    ui->menuView->setCurrentIndex(newIndex);
    ui->menuView->setFocus();
}

void MainWindow::receiveBatteryPercentage(int curBattery)
{
    if (curBattery == 0)
    {
        batteryDied();
    }
    else if (curBattery == 5 && !battery->isCharging()) {
        int currScreenStack = controller->isSessionActive() || controller->isSessionPaused() ? TREATMENT_SCREEN : MENU_SCREEN;
        displayMessage("Battery Critically Low: Please Charge", currScreenStack);
    }
    else if (curBattery < 25)
    {
        QPixmap pixmap(":/images/images/1Battery.png");
        ui->battery->setPixmap(pixmap);
    }
    else if (curBattery < 50)
    {
        QPixmap pixmap(":/images/images/2Battery.png");
        ui->battery->setPixmap(pixmap);
    }
    else if (curBattery < 75)
    {
        QPixmap pixmap(":/images/images/3Battery.png");
        ui->battery->setPixmap(pixmap);
    }
    else
    {
        QPixmap pixmap(":/images/images/4Battery.png");
        ui->battery->setPixmap(pixmap);
    }
}
void MainWindow::batteryDied()
{
    QPixmap pixmap(":/images/images/0Battery.png");
//    qInfo() << "Battery just died.";
    ui->battery->setPixmap(pixmap);
    this->battery->setOn(false);
    this->poweredOn = false;
    turnDeviceScreenOff();
}

void MainWindow::togglePower()
{
    if (this->poweredOn == false)
    {
        this->poweredOn = true;
        this->battery->setOn(true);
    }
    else
    {
        this->poweredOn = false;
        this->battery->setOn(false);
    }
}

void MainWindow::on_chargeButton_clicked()
{
//    qInfo() << "Charging button clicked";
    if (this->battery->isCharging())
    {
        this->battery->setCharging(false);
        ui->chargingBolt->setPixmap(QPixmap(":/images/images/empty.png"));
    }
    else
    {
        this->battery->setCharging(true);
        ui->chargingBolt->setPixmap(QPixmap(":/images/images/charging.png"));
    }
}

void MainWindow::on_powerButton_clicked()
{
//    qInfo() << "Power button clicked. Previously" << (this->poweredOn ? "ON." : "OFF.");
    if (battery->getBattery() > 0)
    {
        if (poweredOn == false)
        {
            ui->powerButton->setIcon(QIcon(":/images/images/PowerOn.png"));
            ui->upButton->setEnabled(true);
            ui->downButton->setEnabled(true);
            ui->playButton->setEnabled(true);
            ui->pauseButton->setEnabled(true);
            ui->stopButton->setEnabled(true);
            ui->selectButton->setEnabled(true);
            ui->electrodeDisconnect->setEnabled(true);
            ui->electrodeReconnect->setEnabled(true);
            ui->ContactLostIndicator->setStyleSheet("background-color: red;");
            if (referenceDateTime.isNull()) {
                ui->dateTimeEdit->setDateTime(customDateTime);
            } else {
                QDateTime currentDateTime = QDateTime::currentDateTime();
                qint64 elapsed = referenceDateTime.msecsTo(currentDateTime);
                ui->dateTimeEdit->setDateTime(customDateTime.addMSecs(elapsed));
            }

            QStringList menuOptions;
            menuOptions << "New Session"
                        << "Session Logs"
                        << "Time and Date";

            // Create a QStringListModel and set the menu options
            QStringListModel *model = new QStringListModel(this);
            model->setStringList(menuOptions);

            // Set the model on the QListView
            ui->menuView->setModel(model);

            QModelIndex firstIndex = ui->menuView->model()->index(0, 0);
            ui->menuView->setCurrentIndex(firstIndex);

            ui->menuView->setFocus();
            togglePower();
        }
        else
        {
            turnDeviceScreenOff();
            togglePower();
        }
    }
}

void MainWindow::turnDeviceScreenOff()
{
    emit stopButtonPressed();
    emit electrodeContactLost();

    ui->screenStack->setCurrentIndex(MENU_SCREEN);
    ui->powerButton->setIcon(QIcon(":/images/images/PowerOff.png"));
    auto *model = dynamic_cast<QStringListModel *>(ui->menuView->model());
    if (model)
    {
        model->setStringList(QStringList()); // Clear the model
    }

    ui->upButton->setEnabled(false);
    ui->downButton->setEnabled(false);
    ui->playButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
    ui->selectButton->setEnabled(false);
    ui->electrodeDisconnect->setEnabled(false);
    ui->electrodeReconnect->setEnabled(false);
    ui->ContactLostIndicator->setStyleSheet("background-color: grey;");
    ui->ContactSecureIndicator->setStyleSheet("background-color: grey;");
    ui->TreatmentIndicator->setStyleSheet("background-color: grey;");
}

void MainWindow::on_selectButton_clicked()
{
    if (ui->screenStack->currentIndex() == TREATMENT_SCREEN) return;

    QModelIndex currentIndex = ui->menuView->currentIndex();

    if (!currentIndex.isValid())
    {
        qDebug() << "No item is selected.";
        return; // No selection made
    }

    int selectedRow = currentIndex.row(); // Get the selected row number
    switch (selectedRow)
    {
    case SELECT_NEW_SESSION:
        // USER REQUESTS NEW SESSION EVENT
        if (ui->screenStack->currentIndex() == MENU_SCREEN && controller->electrodesConnected()) {
            ui->timerLabel->setText("");
            ui->progressBar->setValue(0);
            ui->screenStack->setCurrentIndex(TREATMENT_SCREEN);
            ui->patientStateComboBox->setDisabled(true);
            emit signalNewSession();
        } else {
            displayMessage("Unable to start session: Electrode connection is not secure", MENU_SCREEN);
        }
        break;
    case SELECT_SESSION_LOGS:
        // USER REQUESTS SESSION LOG DISPLAY EVENT
        emit signalSessionLog();
        break;
    case SELECT_TIME_AND_DATE:
        // USER REQUESTS UPDATE DEVICE TIME AND DATE EVENT
        ui->screenStack->setCurrentIndex(SET_DATETIME_SCREEN);
        break;
    // Handle other cases as needed
    default:
        qDebug() << "Selected option is not handled.";
    }
}

void MainWindow::displayMessage(const QString& message, int returnScreen) {
    ui->screenStack->setCurrentIndex(WARNING_MESSAGE_SCREEN);
    ui->warningLabel->setWordWrap(true);
    ui->warningLabel->setText(message);

    // Display warning for 3 seconds
    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, [this, returnScreen]() {
        ui->screenStack->setCurrentIndex(returnScreen);
        if (returnScreen == MENU_SCREEN) {
            ui->menuView->setFocus();
        }
    });
    timer->start(3000);
}

void MainWindow::on_playButton_clicked()
{
    if (controller->isSessionPaused() && controller->electrodesConnected()) {
        emit playButtonPressed();
        if (powerOffTimer->isActive()) {
                  powerOffTimer->stop();
        }
    }
}

void MainWindow::on_pauseButton_clicked()
{
    if (controller->isSessionActive()) {
        emit pauseButtonPressed();
        ui->TreatmentIndicator->setStyleSheet("background-color: grey;");
        displayMessage("Treatment Session Paused", TREATMENT_SCREEN);
        powerOffTimer->start(20000); // 20 seconds timeout
    }
}


void MainWindow::on_stopButton_clicked()
{
    if (controller->isSessionActive()) {
        ui->timerLabel->setText("");
        ui->progressBar->setValue(0);
        ui->TreatmentIndicator->setStyleSheet("background-color: grey;");
        emit stopButtonPressed();
        displayMessage("Treatment Session Stopped", MENU_SCREEN);
    }
}

void MainWindow::updateUITimerAndProgress(const QString &timeString, int progressPercentage)
{
    ui->timerLabel->setText(timeString);
    ui->progressBar->setValue(progressPercentage);
}

// on signal emission from PC this slot function is called for a PC->MainWindow display event to plot graph data to QCustomPlot::graphDisplayPC
void MainWindow::slotDisplayGraphData(const Wave& waveData)
{
    ui->graphDisplayPC->xAxis->setNumberFormat("g"); // use general formatting for tick labels
    ui->graphDisplayPC->xAxis->setNumberPrecision(6); // set the precision of tick labels

    // set the tick count and label spacing for the y-axis
    ui->graphDisplayPC->yAxis->setNumberFormat("g"); // use general formatting for tick labels
    ui->graphDisplayPC->yAxis->setNumberPrecision(6); // set the precision of tick labels

    // set font size for the x-axis tick labels
    QFont xAxisFont = ui->graphDisplayPC->xAxis->tickLabelFont();
    xAxisFont.setPointSize(6); // Adjust the font size as needed
    ui->graphDisplayPC->xAxis->setTickLabelFont(xAxisFont);

    // set font size for the y-axis tick labels
    QFont yAxisFont = ui->graphDisplayPC->yAxis->tickLabelFont();
    yAxisFont.setPointSize(6); // Adjust the font size as needed
    ui->graphDisplayPC->yAxis->setTickLabelFont(yAxisFont);

    ui->graphDisplayPC->yAxis->setLabel("EEG"); // set y-axis label
    ui->graphDisplayPC->xAxis->setLabelFont(QFont("Arial", 4)); // set x-axis label font
    ui->graphDisplayPC->yAxis->setLabelFont(QFont("Arial", 8)); // set y-axis label font
    ui->graphDisplayPC->xAxis->setLabelColor(Qt::black); // set x-axis label color
    ui->graphDisplayPC->yAxis->setLabelColor(Qt::black); // set y-axis label color

    // add a graph if one doesn't exist already
    if (ui->graphDisplayPC->graphCount() == 0)
    {
        ui->graphDisplayPC->addGraph();
    }
    else
    {
        ui->graphDisplayPC->graph(0)->data()->clear(); // clear existing display
    }
    ui->graphDisplayPC->graph(0)->setData(waveData.xPlot, waveData.yPlot);              // set data to the first graph
    ui->graphDisplayPC->xAxis->setRange(waveData.xPlot.first(), waveData.xPlot.last()); // set the range of the x-axis based on the calculated x values
    ui->graphDisplayPC->rescaleAxes(true);                            // rescale the axes to ensure all data points are visible

    // replot the graph
    ui->graphDisplayPC->replot();
}

void MainWindow::on_EEGSampleButton_clicked()
{
    QString electrodeSite = ui->electrodeComboBox->currentText();
    emit signalGetElectrodeEEGWave(electrodeSite);
}

void MainWindow::on_uploadButton_clicked() {
    emit getPreviousSessionDates();
}

void MainWindow::slotDisplaySessionDates(QStringList sessionDates) {
    if (!sessionDates.isEmpty()) {
        QStringListModel* model = new QStringListModel(sessionDates, this);
        ui->prevSessionsList->setModel(model);
    } else {
        ui->prevSessionsList->setModel(new QStringListModel(this));
    }
}

void MainWindow::onSessionDoubleClicked(const QModelIndex &index) {
    QString sessionFileName = index.data(Qt::DisplayRole).toString();
    emit getSessionLogData(sessionFileName);
}

void MainWindow::slotDisplaySessionLogData(QStringList sessionLogData) {
    QStringListModel* model = new QStringListModel(sessionLogData, this);
    ui->sessionLogView->setModel(model);
}

void MainWindow::on_electrodeDisconnect_clicked()
{
    ui->ContactSecureIndicator->setStyleSheet("background-color: grey;");
    ui->ContactLostIndicator->setStyleSheet("background-color: red;");
    ui->TreatmentIndicator->setStyleSheet("background-color: grey");
    emit electrodeContactLost();
    if (controller->isSessionActive() || controller->isSessionPaused()) {
        displayMessage("Electrode Contact Lost, Please Reconnect", TREATMENT_SCREEN);
        powerOffTimer->start(20000);
    } else {
        ui->menuView->setFocus();
    }
}

void MainWindow::on_electrodeReconnect_clicked()
{
    ui->ContactSecureIndicator->setStyleSheet("background-color: blue;");
    ui->ContactLostIndicator->setStyleSheet("background-color: grey;");
    emit electrodeContactRegained();
    ui->menuView->setFocus();


    if (powerOffTimer->isActive()) {
        powerOffTimer->stop();
    }
}

void MainWindow::slotTreatmentSessionComplete()
{
    ui->patientStateComboBox->setDisabled(false);
    displayMessage("Treatment Session Complete", MENU_SCREEN);
}

void MainWindow::on_patientStateComboBox_currentIndexChanged() {
    qInfo() << ui->patientStateComboBox->currentText();
    controller->setPatientState(ui->patientStateComboBox->currentText());
}

void MainWindow::slotTreatmentApplicationStarted() {
    ui->TreatmentIndicator->setStyleSheet("background-color: grey");

    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, [this]() {
        ui->TreatmentIndicator->setStyleSheet("background-color: green");
    });
    timer->start(500);
}

void MainWindow::slotTreatmentApplicationFinished() {
    ui->TreatmentIndicator->setStyleSheet("background-color: grey");
}


void MainWindow::on_menuButton_clicked() {
    if (ui->screenStack->currentIndex() == TREATMENT_SCREEN) return;

    if (ui->screenStack->currentIndex() == SET_DATETIME_SCREEN) {
        customDateTime = ui->dateTimeEdit->dateTime();
        referenceDateTime = QDateTime::currentDateTime();
        ui->screenStack->setCurrentIndex(MENU_SCREEN);
        emit signalTimeAndDate(customDateTime, referenceDateTime);
    }
}

