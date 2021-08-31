#include "clagcodeanalyse.h"

#include <QDebug>
#include <QtMath>

ClaGcodeAnalyse::ClaGcodeAnalyse(QObject *parent) : QObject(parent)
{
    posX = 0;
    posY = 0;
    moveMode = EnumRelative;
}

ClaGcodeAnalyse::~ClaGcodeAnalyse()
{

}

//===============================================================================================
double ClaGcodeAnalyse::FunGetDistanceBetweenTwoPoint(double x1, double y1, double x2, double y2)
{
    double distance = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
    return distance;
}

void ClaGcodeAnalyse::FunSplitCommondBySpace(QString nowStr, QList<QString> &splitStrVec)
{
    QStringList nowStrList = nowStr.split(' ');
    for(int i = 0; i < nowStrList.size(); i++)
    {
        if(!(nowStrList.at(i).isEmpty()))
        {
            if(nowStrList.at(i).right(1) == "\n")
                splitStrVec.push_back(nowStrList[i].left(nowStrList[i].size() - 2));
            else
                splitStrVec.push_back(nowStrList[i]);
        }
    }
}

int ClaGcodeAnalyse::FunGetAngleBetweenYlineAndIt(float x, float y)
{
    int retAngle = atan2(x, y) * 180 / PI;
    return retAngle;
}
//===============================================================================================
bool ClaGcodeAnalyse::FunGetMotion(QList<QString> &splitStrVec, int &nowMotion, int &allMotionCount)
{
    //取出前面所有以G/g开头的字符串
    QList<QString> allMotionVec;
    for(int i = 0; i < splitStrVec.size(); i++)
        if(splitStrVec.at(i).at(0) == 'g' || splitStrVec.at(i).at(0) == 'G')
        {
            allMotionVec.push_back(splitStrVec.at(i));
        }
        else
            break;
    //如果以G/g开头的字符串一个都没有，直接返回false
    if(0 == allMotionVec.size())
    {
        allMotionCount = 0;
        return false;
    }

    allMotionCount = allMotionVec.size();

    //取出的以G/g开头的字符串中，长度必须是2或3，不是直接返回false（有一个不是就错误）
    for(int i = 0; i < allMotionVec.size(); i++)
    {
        if(!(allMotionVec.at(i).size() == 2 || allMotionVec.at(i).size() == 3))
            return false;
    }

    //判断是否有用两次G0/G1/G2/G3，或两次G90/G91（可以有0次或1次）
    int g0123Num = 0;
    int g9091Num = 0;
    QString afterGstr;
    for(int i = 0; i < allMotionVec.size(); i++)
    {
        afterGstr = allMotionVec.at(i).mid(1,allMotionVec.at(i).size() - 1);
        if( afterGstr == "0" || afterGstr == "00" ||
            afterGstr == "1" || afterGstr == "01" ||
            afterGstr == "2" || afterGstr == "02" ||
            afterGstr == "3" || afterGstr == "03")
            g0123Num++;

        if(afterGstr == "90" || afterGstr == "91")
            g9091Num++;
    }
    if(g0123Num > 1)
        return false;
    if(g9091Num > 1)
        return false;

    //针对所有Gx做操作
    for(int i = 0; i < allMotionVec.size(); i++)
    {
        afterGstr = allMotionVec.at(i).mid(1,allMotionVec.at(i).size() - 1);
        if( afterGstr == "0" || afterGstr == "00" ||
            afterGstr == "1" || afterGstr == "01" ||
            afterGstr == "2" || afterGstr == "02" ||
            afterGstr == "3" || afterGstr == "03")
        {
            nowMotion = afterGstr.toInt();
        }
        else if(afterGstr == "17")
        {
            Fun17Motion();
        }
        else if(afterGstr == "28")
        {
            Fun28Motion();
        }
        else if(afterGstr == "90")
        {
            Fun90Motion();
        }
        else if(afterGstr == "91")
        {
            Fun91Motion();
        }
        else
        {
            return false;
        }
    }

    return true;
}
//===============================================================================================
bool ClaGcodeAnalyse::FunAnalyseValueStr(QString valueStr)
{
    //先去掉最后一个回车
    if(valueStr.right(1) == "\n")
        valueStr = valueStr.left(valueStr.size() - 2);

    //有没有不正经的字符
    for(int i = 0; i < valueStr.size(); i++)
    {
        if( !((valueStr.at(i)>='0' && valueStr.at(i)<='9') ||
            (valueStr.at(i) == '-') ||
            (valueStr.at(i) == '.')) )
            return false;
    }

    //计算.和-出现的次数(只能是0或1次)
    int dotCount = 0;
    int subCount = 0;
    for(int i = 0; i < valueStr.size(); i++)
    {
        if(valueStr.at(i) == '.')
            dotCount++;

        if(valueStr.at(i) == '-')
            subCount++;
    }
    if(dotCount > 1)
        return false;
    if(subCount > 1)
        return false;

    //判断-出现的位置合不合法
    if(valueStr.indexOf('-') > 0)
        return false;

    //判断.出现的位置合不合法
    if(valueStr.indexOf('-') == 0)//有-
    {

        if((valueStr.indexOf('.') < 2 || valueStr.indexOf('.') > valueStr.size() - 1 - 1)
                && (valueStr.indexOf('.') != -1))
            return false;
    }
    else//没有-
    {
        if((valueStr.indexOf('.') < 1 || valueStr.indexOf('.') > valueStr.size() - 1 - 1)
                && (valueStr.indexOf('.') != -1))
            return false;
    }
    return true;
}
//===============================================================================================
bool ClaGcodeAnalyse::Fun00Motion(QList<QString> &splitStrVec, int allMotionCount)
{
    //判断是不是两个
    if(2 != splitStrVec.size() - allMotionCount)
        return false;

    //判断第一个开头是不是x，以及第二个开头是不是y
    if(!(splitStrVec.at(allMotionCount).left(1) == "x" || splitStrVec.at(allMotionCount).left(1) == "X"))
        return false;
    if(!(splitStrVec.at(allMotionCount + 1).left(1) == "y" || splitStrVec.at(allMotionCount + 1).left(1) == "Y"))
        return false;

    //拿X后的Value,并判断合不合法
    QString xValueStr = splitStrVec.at(allMotionCount).mid(1,splitStrVec.at(allMotionCount).size() - 1);
    if(!FunAnalyseValueStr(xValueStr))
        return false;

    //拿Y后的Value，并判断合不合法
    QString yValueStr = splitStrVec.at(allMotionCount + 1).mid(1,splitStrVec.at(allMotionCount + 1).size() - 1);

    if(!FunAnalyseValueStr(yValueStr))
        return false;

    if(EnumAbsolute == this->moveMode)//EnumAbsolute
    {
        this->posX = xValueStr.toFloat();
        this->posY = yValueStr.toFloat();
    }
    else//EnumRelative
    {
        this->posX = this->posX + xValueStr.toFloat();
        this->posY = this->posY + yValueStr.toFloat();
    }

    return true;
}

bool ClaGcodeAnalyse::Fun01Motion(QList<QString> &splitStrVec, int allMotionCount, QList<struShapeConf> &retShapeConf)
{
    //判断是不是两个
    if(2 != splitStrVec.size() - allMotionCount)
        return false;

    //判断第一个开头是不是x，以及第二个开头是不是y
    if(!(splitStrVec.at(allMotionCount).left(1) == "x" || splitStrVec.at(allMotionCount).left(1) == "X"))
        return false;
    if(!(splitStrVec.at(allMotionCount + 1).left(1) == "y" || splitStrVec.at(allMotionCount + 1).left(1) == "Y"))
        return false;

    //拿X后的Value,并判断合不合法
    QString xValueStr = splitStrVec.at(allMotionCount).mid(1,splitStrVec.at(allMotionCount).size() - 1);
    if(!FunAnalyseValueStr(xValueStr))
        return false;

    //拿Y后的Value，并判断合不合法
    QString yValueStr = splitStrVec.at(allMotionCount + 1).mid(1,splitStrVec.at(allMotionCount + 1).size() - 1);
    if(!FunAnalyseValueStr(yValueStr))
        return false;

    struShapeConf tmpConf;
    if(EnumAbsolute == this->moveMode)//EnumAbsolute
    {
        tmpConf.typeConf = 0;
        tmpConf.directConf = 0;
        tmpConf.xConf = (int)(this->posX);
        tmpConf.yConf = (int)(this->posY);
        tmpConf.widthConf = (int)(xValueStr.toFloat());
        tmpConf.highConf = (int)(yValueStr.toFloat());
        tmpConf.pointConf = 0;
        tmpConf.angleConf = 0;

        this->posX = xValueStr.toFloat();
        this->posY = yValueStr.toFloat();
    }
    else//EnumRelative
    {
        tmpConf.typeConf = 0;
        tmpConf.directConf = 0;
        tmpConf.xConf = (int)(this->posX);
        tmpConf.yConf = (int)(this->posY);
        tmpConf.widthConf = (int)(this->posX + xValueStr.toFloat());
        tmpConf.highConf = (int)(this->posY + yValueStr.toFloat());
        tmpConf.pointConf = 0;
        tmpConf.angleConf = 0;

        this->posX = this->posX + xValueStr.toFloat();
        this->posY = this->posY + yValueStr.toFloat();
    }
    retShapeConf.push_back(tmpConf);

    return true;
}

bool ClaGcodeAnalyse::Fun02Motion(QList<QString> &splitStrVec, int allMotionCount, QList<struShapeConf> &retShapeConf)
{
    //判断参数是不是4个
    if(4 != splitStrVec.size() - allMotionCount)
        return false;

    //判断第一个开头是不是x，以及第二个开头是不是y
    if(!(splitStrVec.at(allMotionCount).left(1) == "x" || splitStrVec.at(allMotionCount).left(1) == "X"))
        return false;
    if(!(splitStrVec.at(allMotionCount + 1).left(1) == "y" || splitStrVec.at(allMotionCount + 1).left(1) == "Y"))
        return false;
    //拿X后的Value,并判断合不合法
    QString xValueStr = splitStrVec.at(allMotionCount).mid(1,splitStrVec.at(allMotionCount).size() - 1);
    if(!FunAnalyseValueStr(xValueStr))
        return false;
    //拿Y后的Value，并判断合不合法
    QString yValueStr = splitStrVec.at(allMotionCount + 1).mid(1,splitStrVec.at(allMotionCount + 1).size() - 1);
    if(!FunAnalyseValueStr(yValueStr))
        return false;

    //根据实际项目数判定格式是否正确
    QString iValueStr, jValueStr;
    if(!(splitStrVec.at(allMotionCount + 2).left(1) == "i" || splitStrVec.at(allMotionCount + 2).left(1) == "I"))
        return false;
    if(!(splitStrVec.at(allMotionCount + 3).left(1) == "j" || splitStrVec.at(allMotionCount + 3).left(1) == "J"))
        return false;
    //取出数据项判断是否合法
    iValueStr = splitStrVec.at(allMotionCount + 2).mid(1,splitStrVec.at(allMotionCount + 2).size() - 1);
    if(!FunAnalyseValueStr(iValueStr))
        return false;
    jValueStr = splitStrVec.at(allMotionCount + 3).mid(1,splitStrVec.at(allMotionCount + 3).size() - 1);
    if(!FunAnalyseValueStr(jValueStr))
        return false;
    //先根据i,j计算圆心的位置
    float circleCenterX;
    float circleCenterY;
    if(EnumAbsolute == this->moveMode)
    {
        circleCenterX = iValueStr.toFloat();
        circleCenterY = jValueStr.toFloat();
    }
    else
    {
        circleCenterX = this->posX + iValueStr.toFloat();
        circleCenterY = this->posY + jValueStr.toFloat();
    }
    //根据圆心的位置判断半径是否一致
    float rBeginToCenter = FunGetDistanceBetweenTwoPoint((double)this->posX, (double)this->posY, (double)circleCenterX, (double)circleCenterY);
    float rEndToCenter = FunGetDistanceBetweenTwoPoint(xValueStr.toDouble(), yValueStr.toDouble(), (double)circleCenterX, (double)circleCenterY);
    if(rBeginToCenter != rEndToCenter)
        return false;

    int beginPoint = -1 * (FunGetAngleBetweenYlineAndIt(this->posX - circleCenterX, this->posY - circleCenterY) - 90);
    int arcAngle =  abs(-1 * (FunGetAngleBetweenYlineAndIt(xValueStr.toFloat() - circleCenterX, yValueStr.toFloat() - circleCenterY) - 90)
                    -
                    -1 * (FunGetAngleBetweenYlineAndIt(this->posX - circleCenterX, this->posY - circleCenterY) - 90));

    if(FunGetAngleBetweenYlineAndIt(xValueStr.toFloat() - circleCenterY, yValueStr.toFloat() - circleCenterY) > 0)
    {
        arcAngle = arcAngle;
    }
    else
    {
        arcAngle = 360 - arcAngle;
    }
    //顺时针
    arcAngle *= -1;

    struShapeConf tmpConf;
    tmpConf.typeConf = 2;
    tmpConf.directConf = 0;
    tmpConf.xConf = (int)(circleCenterX - rBeginToCenter);
    tmpConf.yConf = (int)(circleCenterY - rBeginToCenter);
    tmpConf.widthConf = (int)(rBeginToCenter * 2);
    tmpConf.highConf = (int)(rBeginToCenter * 2);
    tmpConf.pointConf = beginPoint;
    tmpConf.angleConf = arcAngle;
    retShapeConf.push_back(tmpConf);

    return true;
}
bool ClaGcodeAnalyse::Fun03Motion(QList<QString> &splitStrVec, int allMotionCount, QList<struShapeConf> &retShapeConf)
{
    //判断参数是不是4个
    if(4 != splitStrVec.size() - allMotionCount)
        return false;

    //判断第一个开头是不是x，以及第二个开头是不是y
    if(!(splitStrVec.at(allMotionCount).left(1) == "x" || splitStrVec.at(allMotionCount).left(1) == "X"))
        return false;
    if(!(splitStrVec.at(allMotionCount + 1).left(1) == "y" || splitStrVec.at(allMotionCount + 1).left(1) == "Y"))
        return false;
    //拿X后的Value,并判断合不合法
    QString xValueStr = splitStrVec.at(allMotionCount).mid(1,splitStrVec.at(allMotionCount).size() - 1);
    if(!FunAnalyseValueStr(xValueStr))
        return false;
    //拿Y后的Value，并判断合不合法
    QString yValueStr = splitStrVec.at(allMotionCount + 1).mid(1,splitStrVec.at(allMotionCount + 1).size() - 1);
    if(!FunAnalyseValueStr(yValueStr))
        return false;

    //根据实际项目数判定格式是否正确
    QString iValueStr, jValueStr;
    if(!(splitStrVec.at(allMotionCount + 2).left(1) == "i" || splitStrVec.at(allMotionCount + 2).left(1) == "I"))
        return false;
    if(!(splitStrVec.at(allMotionCount + 3).left(1) == "j" || splitStrVec.at(allMotionCount + 3).left(1) == "J"))
        return false;
    //取出数据项判断是否合法
    iValueStr = splitStrVec.at(allMotionCount + 2).mid(1,splitStrVec.at(allMotionCount + 2).size() - 1);
    if(!FunAnalyseValueStr(iValueStr))
        return false;
    jValueStr = splitStrVec.at(allMotionCount + 3).mid(1,splitStrVec.at(allMotionCount + 3).size() - 1);
    if(!FunAnalyseValueStr(jValueStr))
        return false;
    //先根据i,j计算圆心的位置
    float circleCenterX;
    float circleCenterY;
    if(EnumAbsolute == this->moveMode)
    {
        circleCenterX = iValueStr.toFloat();
        circleCenterY = jValueStr.toFloat();
    }
    else
    {
        circleCenterX = this->posX + iValueStr.toFloat();
        circleCenterY = this->posY + jValueStr.toFloat();
    }
    //根据圆心的位置判断半径是否一致
    float rBeginToCenter = FunGetDistanceBetweenTwoPoint((double)this->posX, (double)this->posY, (double)circleCenterX, (double)circleCenterY);
    float rEndToCenter = FunGetDistanceBetweenTwoPoint(xValueStr.toDouble(), yValueStr.toDouble(), (double)circleCenterX, (double)circleCenterY);
    if(rBeginToCenter != rEndToCenter)
        return false;

    int beginPoint = -1 * (FunGetAngleBetweenYlineAndIt(this->posX - circleCenterX, this->posY - circleCenterY) - 90);
    int arcAngle =  abs(-1 * (FunGetAngleBetweenYlineAndIt(xValueStr.toFloat() - circleCenterX, yValueStr.toFloat() - circleCenterY) - 90)
                    -
                    -1 * (FunGetAngleBetweenYlineAndIt(this->posX - circleCenterX, this->posY - circleCenterY) - 90));

    if(FunGetAngleBetweenYlineAndIt(xValueStr.toFloat() - circleCenterY, yValueStr.toFloat() - circleCenterY) > 0)
    {
        arcAngle = arcAngle;
    }
    else
    {
        arcAngle = 360 - arcAngle;
    }
    //逆时针
    arcAngle *= -1;
    arcAngle = 360 - abs(arcAngle);

    struShapeConf tmpConf;
    tmpConf.typeConf = 2;
    tmpConf.directConf = 0;
    tmpConf.xConf = (int)(circleCenterX - rBeginToCenter);
    tmpConf.yConf = (int)(circleCenterY - rBeginToCenter);
    tmpConf.widthConf = (int)(rBeginToCenter * 2);
    tmpConf.highConf = (int)(rBeginToCenter * 2);
    tmpConf.pointConf = beginPoint;
    tmpConf.angleConf = arcAngle;
    retShapeConf.push_back(tmpConf);

    return true;
}

void ClaGcodeAnalyse::Fun17Motion(void)
{

}
void ClaGcodeAnalyse::Fun28Motion(void)
{
    this->posX = 0;
    this->posY = 0;
}
void ClaGcodeAnalyse::Fun90Motion(void)
{
    this->moveMode = EnumAbsolute;
}
void ClaGcodeAnalyse::Fun91Motion(void)
{
    this->moveMode = EnumRelative;
}
//===============================================================================================
void ClaGcodeAnalyse::FunGcodeAnalyse(QList<QString> allBuf, QList<struShapeConf> &retShapeConf)
{
    for(int i = 0; i < allBuf.size(); i++)
    {
        QList<QString> splitStrVec;

        FunSplitCommondBySpace(allBuf[i], splitStrVec);

        if(0 == splitStrVec.size())
            continue;

        int nowMotion;
        int allMotionCount;
        bool analyResult = FunGetMotion(splitStrVec, nowMotion, allMotionCount);
        if(false == analyResult)
            continue;

        //GcoddeMotion整完了，获取参数
        if(0 == nowMotion)
        {
            Fun00Motion(splitStrVec, allMotionCount);
        }
        else if(1 == nowMotion)
        {
            Fun01Motion(splitStrVec, allMotionCount, retShapeConf);
        }
        else if(2 == nowMotion)
        {
            Fun02Motion(splitStrVec, allMotionCount, retShapeConf);
        }
        else if(3 == nowMotion)
        {
            Fun03Motion(splitStrVec, allMotionCount, retShapeConf);
        }
    }
}
