#include "threadgetxy.h"
#include <QDebug>

ThreadGetXY::ThreadGetXY(X_Net *outService)
{
    service = outService;
}

void ThreadGetXY::run()
{
    stopFlag = false;
    while(1)
    {
        ClaTimer::TimeSleep(10);

        double posiX, posiY;
        int resultX, resultY;
        service->XNETRead(XNET_D, 20044, 4, &posiX, resultX);
        ClaTimer::TimeSleep(10);
        service->XNETRead(XNET_D, 20244, 4, &posiY, resultY);

        //qDebug() << "X:" << posiX << " Y:" << posiY;
        emit SigGetedXAndY(posiX, posiY);

        ClaTimer::TimeSleep(10);

        if(stopFlag == true)
            break;
    }
}

