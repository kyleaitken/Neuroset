#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug() << QDir::currentPath();
    ui->setupUi(this);
    ui->powerButton->setIcon(QIcon(":/images/images/PowerOn.png"));
    ui->stopButton->setIcon(QIcon(":/images/images/stop.png"));
    ui->playButton->setIcon(QIcon(":/images/images/play.png"));
    ui->pauseButton->setIcon(QIcon(":/images/images/pause.png"));

    QPixmap pixmap(":/images/images/4Battery.png");


    ui->battery->setPixmap(pixmap);
}

MainWindow::~MainWindow()
{
    delete ui;
}

