#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{

    // Setting up UI icons
    ui->setupUi(this);
    ui->powerButton->setIcon(QIcon(":/images/images/PowerOn.png"));
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

    // starts a new session when user presses play - needs adjusting to respond to pressing when when 'Start Session' is highlighted on the UI
    connect(ui->playButton, &QPushButton::released, controller, &Controller::startNewSession);
    connect(ui->pauseButton, &QPushButton::released, controller, &Controller::pauseSession);

    controllerThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}
