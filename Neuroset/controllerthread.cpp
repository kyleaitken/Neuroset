#include "controllerthread.h"

ControllerThread::ControllerThread(QObject* parent) : QThread(parent)
{

}

void ControllerThread::run() {
    qInfo() << "Controller thread running on id: " << QThread::currentThreadId();
    while (!isInterruptionRequested()) {

    }
}
