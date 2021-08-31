#include "cladrawbyplc.h"
#include <QDebug>

ClaDrawByPlc::ClaDrawByPlc(QObject *parent) : QObject(parent)
{

}

ClaDrawByPlc::~ClaDrawByPlc()
{

}

void ClaDrawByPlc::FunWriteStruToPlc(X_Net *service, QList<struPlcLineConf> &plcLineConfVec)
{
    short tmpnum;

    //先写总行数
    tmpnum = plcLineConfVec.size();
    qDebug() << "Line Count:" << tmpnum;
    qDebug() << "Write Result:" << service->WriteRegs(XNet_HD, HD_LINE_BEGIN, 1, &tmpnum);

    //再确定是否重新载入 0:继续载入 1：重新载入
    tmpnum = 0;
    service->WriteRegs(XNet_HD, HD_LINE_BEGIN + 1, 1, &tmpnum);

    //然后开始写所有行数的数据 行下标：i 行数:i + 1
    for(int i = 0; i < plcLineConfVec.size(); i++)
    {
        //写行号
        tmpnum = plcLineConfVec.at(i).lineNum;
        service->WriteRegs(XNet_HD, (HD_LINE_BEGIN + 10 + 60 * i), 1, &tmpnum);

        //写类型
        tmpnum = plcLineConfVec.at(i).moveType;
        service->WriteRegs(XNet_HD, (HD_LINE_BEGIN + 12 + 60 * i), 1, &tmpnum);

        //写数据类型
        tmpnum = 0;
        service->WriteRegs(XNet_HD, (HD_LINE_BEGIN + 13 + 60 * i), 1, &tmpnum);

        //写参数 //0:ptp 1:line 2:circle 200:end
        if(0 == plcLineConfVec.at(i).moveType || 1 == plcLineConfVec.at(i).moveType)
        {
            double tmpPosi;
            double tmpSpeed;
            int result;

            //目标X的坐标
            tmpPosi = plcLineConfVec.at(i).targetX;
            service->XNETWrite(XNET_HD, (HD_LINE_BEGIN + 16 + 60 * i), 4, &tmpPosi, result);

            //目标Y的坐标
            tmpPosi = plcLineConfVec.at(i).targetY;
            service->XNETWrite(XNET_HD, (HD_LINE_BEGIN + 20 + 60 * i), 4, &tmpPosi, result);

            //目标Z的坐标
            tmpPosi = plcLineConfVec.at(i).targetZ;
            service->XNETWrite(XNET_HD, (HD_LINE_BEGIN + 24 + 60 * i), 4, &tmpPosi, result);

            //目标速度
            tmpSpeed = DRAW_SPEED;
            service->XNETWrite(XNET_HD, (HD_LINE_BEGIN + 64 + 60 * i), 4, &tmpSpeed, result);
        }
        else if(2 == plcLineConfVec.at(i).moveType)
        {
            //圆和圆弧的憋着急
        }
        else if(200 == plcLineConfVec.at(i).moveType)
        {

            return;
        }
    }
}

void ClaDrawByPlc::FunGetPlcLineConf(X_Net *service, QList<struPlcLineConf> &plcLineConfVec)
{
    short numMOn = 256;
    short numMOff = 0;

    //先把东西写到寄存器去吧
    FunWriteStruToPlc(service, plcLineConfVec);
    ClaTimer::TimeSleep(100);

    //从寄存器载入缓冲区
    service->WriteRegs(XNet_M, 10, 1, &numMOn);
    ClaTimer::TimeSleep(100);
    service->WriteRegs(XNet_M, 10, 1, &numMOff);
    ClaTimer::TimeSleep(100);

    //用缓冲区的数据开始画图
    service->WriteRegs(XNet_M, 12, 1, &numMOn);
    ClaTimer::TimeSleep(100);
    service->WriteRegs(XNet_M, 12, 1, &numMOff);
    ClaTimer::TimeSleep(100);

}
