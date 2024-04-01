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
    connect(controller, &Controller::updateTimerView, this, &MainWindow::updateUISessionTimer);

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

void MainWindow::togglePower()
{
    if (this->poweredOn == false)
    {
        this->poweredOn = true;
    }
    else
    {
        this->poweredOn = false;
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
    emit stopButtonPressed();
}

void MainWindow::updateUISessionTimer(const QString& timeString) {
    ui->timerLabel->setText(timeString);
}
