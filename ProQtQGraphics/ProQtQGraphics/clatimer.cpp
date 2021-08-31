#include "clatimer.h"
#include <QTime>
#include <QApplication>

ClaTimer::ClaTimer(QObject *parent) : QObject(parent)
{

}

ClaTimer::~ClaTimer()
{

}

//自定义延时函数 mesc单位为毫秒
void ClaTimer::TimeSleep(unsigned int mesc)
{
    QTime reachTime = QTime::currentTime().addMSecs(mesc);
    while(QTime::currentTime() < reachTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}
