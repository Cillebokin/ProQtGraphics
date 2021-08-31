#ifndef THREADGETXY_H
#define THREADGETXY_H

#include <QObject>
#include <QThread>

#include "clatimer.h"

#include "x_net.h"

class ThreadGetXY : public QThread
{
    Q_OBJECT
public:
    ThreadGetXY(X_Net *outService);
    bool stopFlag;

private:
    X_Net *service;

protected:
    void run();

signals:
    void SigGetedXAndY(double posiX, double posiY);

public slots:
};

#endif // THREADGETXY_H
