#ifndef CLADRAWBYPLC_H
#define CLADRAWBYPLC_H

#include <QObject>

#include "x_net.h"
#include "clatimer.h"
#include "clashapeitem.h"

#define HD_LINE_BEGIN 1000
#define DRAW_SPEED 40000

class ClaDrawByPlc : public QObject
{
    Q_OBJECT
public:
    explicit ClaDrawByPlc(QObject *parent = 0);
    ~ClaDrawByPlc();

    void FunWriteStruToPlc(X_Net *service, QList<struPlcLineConf> &plcLineConfVec);
    void FunGetPlcLineConf(X_Net *service, QList<struPlcLineConf> &plcLineConfVec);

private:



signals:

public slots:
};

#endif // CLADRAWBYPLC_H
