#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{

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

    QThread *controllerThread = new QThread(this);
    Controller *controller = new Controller();
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

    ui->upButton->setEnabled(false);
    ui->downButton->setEnabled(false);
    ui->playButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
    ui->selectButton->setEnabled(false);

    // signals to handle each menu selection
    connect(this, &MainWindow::signalNewSession, controller, &Controller::startNewSession);
    connect(this, &MainWindow::signalSessionLog, controller, &Controller::sessionLog);
    connect(this, &MainWindow::signalTimeAndDate, controller, &Controller::timeAndDate);
    connect(this, &MainWindow::playButtonPressed, controller, &Controller::resumeTreatmentSession);
    connect(this, &MainWindow::pauseButtonPressed, controller, &Controller::pauseSession);
    connect(this, &MainWindow::stopButtonPressed, controller, &Controller::stopSession);
    connect(batterythread, &BatteryThread::tellMainWindowBatteryPercentage, this, &MainWindow::receiveBatteryPercentage);
    connect(controller, &Controller::updateTimerAndProgressDisplay, this, &MainWindow::updateUITimerAndProgress);

    /***************  PC  ******************/
    pc = new PC(this);                                                                                               // external device to test Neuroset device with display window for graphing EEG   [ MEMORY ALLOC ]
    connect(pc, SIGNAL(signalDisplayGraphData(QVector<double>)), this, SLOT(slotDisplayGraphData(QVector<double>))); // connecting PC signal to MAINWINDOW slot for PC -> MAINWINDOW ui display event
    /***************  PC  ******************/

    controllerThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_upButton_clicked()
{
    // TODO: check state first, should not be in an active session
    int currentRow = ui->menuView->currentIndex().row();
    int previousRow = currentRow > 0 ? currentRow - 1 : 0;
    QModelIndex newIndex = ui->menuView->model()->index(previousRow, 0);
    ui->menuView->setCurrentIndex(newIndex);
    ui->menuView->setFocus();
}

void MainWindow::on_downButton_clicked()
{
    // TODO: check state first, should not be in an active session
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
    qInfo() << "Battery just died.";
    ui->battery->setPixmap(pixmap);
    this->battery->setOn(false);
    turnDeviceOff();
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
    qInfo() << "Charging button clicked";
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
    if (poweredOn == false)
    {
        ui->powerButton->setIcon(QIcon(":/images/images/PowerOn.png"));
        ui->upButton->setEnabled(true);
        ui->downButton->setEnabled(true);
        ui->playButton->setEnabled(true);
        ui->pauseButton->setEnabled(true);
        ui->stopButton->setEnabled(true);
        ui->selectButton->setEnabled(true);

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
        turnDeviceOff();
    }
}

void MainWindow::turnDeviceOff()
{
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

    togglePower();
}

void MainWindow::on_selectButton_clicked()
{
    // TODO: check state first, should not be a current session active
    QModelIndex currentIndex = ui->menuView->currentIndex();

    if (!currentIndex.isValid())
    {
        qDebug() << "No item is selected.";
        return; // No selection made
    }

    int selectedRow = currentIndex.row(); // Get the selected row number
    switch (selectedRow)
    {
    case 0:
        emit signalNewSession();
        ui->screenStack->setCurrentIndex(1);
        break;
    case 1:
        emit signalSessionLog();
        break;
    case 2:
        emit signalTimeAndDate();
        break;
    // Handle other cases as needed
    default:
        qDebug() << "Selected option is not handled.";
    }
}

void MainWindow::on_playButton_clicked()
{
    // TODO: check state of controller first, should be in paused state (if using a select button)
    qInfo() << "Play button clicked";
    emit playButtonPressed();
}

void MainWindow::on_pauseButton_clicked()
{
    // TODO: check state of controller, should be in an active session
    emit pauseButtonPressed();
}

void MainWindow::on_stopButton_clicked()
{
    // TODO: check state of controller first, should be in an active session
    ui->screenStack->setCurrentIndex(0);
    ui->timerLabel->setText("23:00");
    ui->progressBar->setValue(0);
    emit stopButtonPressed();
}

void MainWindow::updateUITimerAndProgress(const QString &timeString, int progressPercentage)
{
    ui->timerLabel->setText(timeString);
    ui->progressBar->setValue(progressPercentage);
}

// on signal emission from PC this slot function is called for a PC->MainWindow display event to plot graph data to QCustomPlot::graphDisplayPC
void MainWindow::slotDisplayGraphData(QVector<double> yPlot)
{
    ui->graphDisplayPC->xAxis->setNumberFormat("g");  // use general formatting for tick labels
    ui->graphDisplayPC->xAxis->setNumberPrecision(6); // set the precision of tick labels

    // set the tick count and label spacing for the y-axis
    ui->graphDisplayPC->yAxis->setNumberFormat("g");  // use general formatting for tick labels
    ui->graphDisplayPC->yAxis->setNumberPrecision(6); // set the precision of tick labels

    // set font size for the x-axis tick labels
    QFont xAxisFont = ui->graphDisplayPC->xAxis->tickLabelFont();
    xAxisFont.setPointSize(6); // Adjust the font size as needed
    ui->graphDisplayPC->xAxis->setTickLabelFont(xAxisFont);

    // set font size for the y-axis tick labels
    QFont yAxisFont = ui->graphDisplayPC->yAxis->tickLabelFont();
    yAxisFont.setPointSize(6); // Adjust the font size as needed
    ui->graphDisplayPC->yAxis->setTickLabelFont(yAxisFont);

    ui->graphDisplayPC->yAxis->setLabel("SITE TITLE");          // set y-axis label
    ui->graphDisplayPC->xAxis->setLabelFont(QFont("Arial", 4)); // set x-axis label font
    ui->graphDisplayPC->yAxis->setLabelFont(QFont("Arial", 8)); // set y-axis label font
    ui->graphDisplayPC->xAxis->setLabelColor(Qt::black);        // set x-axis label color
    ui->graphDisplayPC->yAxis->setLabelColor(Qt::black);        // set y-axis label color

    // following is adding the points to the graph and scalling the graph
    int numDataPoints = yPlot.size();

    QVector<double> xPlot;
    for (int i = 0; i < numDataPoints; ++i)
    {
        double x = i * SAMPLE_RATE + DATA_START; // DATA_START  points <20 look have an inconsistent look
        xPlot.push_back(x);
    }

    // add a graph if one doesn't exist already
    if (ui->graphDisplayPC->graphCount() == 0)
    {
        ui->graphDisplayPC->addGraph();
    }
    ui->graphDisplayPC->graph(0)->setData(xPlot, yPlot);              // set data to the first graph
    ui->graphDisplayPC->xAxis->setRange(xPlot.first(), xPlot.last()); // set the range of the x-axis based on the calculated x values
    ui->graphDisplayPC->rescaleAxes(true);                            // rescale the axes to ensure all data points are visible

    // replot the graph
    ui->graphDisplayPC->replot();
}

void MainWindow::on_EEGSampleButton_clicked()
{
    pc->displayElectrodeEEG("Fp1");
}
