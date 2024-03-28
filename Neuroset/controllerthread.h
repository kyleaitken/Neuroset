#ifndef CONTROLLERTHREAD_H
#define CONTROLLERTHREAD_H

#include <QThread>
#include <QDebug>

class ControllerThread : public QThread
{
    Q_OBJECT
public:
    explicit ControllerThread(QObject* parent = nullptr);
    void run() override;
};

#endif // CONTROLLERTHREAD_H
