#ifndef CLAGCODEANALYSE_H
#define CLAGCODEANALYSE_H

#include <QObject>
#include <QPainterPath>

#include "clashapeitem.h"

#define PI 3.1415

class ClaGcodeAnalyse : public QObject
{
    Q_OBJECT
public:
    explicit ClaGcodeAnalyse(QObject *parent = 0);
    ~ClaGcodeAnalyse();

    enum EnumGcode{G00 = 0, G01 = 1, G02 = 2, G03 = 3};
    enum EnumGmode{G17 = 17, G28 = 28, G90 = 90, G91 = 91};

    enum EnumNowMotion{EnumNotG = -1, EnumMotionNumErr = -2};
    enum EnumMoveMode{EnumAbsolute = 0, EnumRelative = 1};

    float posX;
    float posY;
    bool moveMode;

    void FunGcodeAnalyse(QList<QString> allBuf, QList<struShapeConf> &retShapeConf);

private:
    double FunGetDistanceBetweenTwoPoint(double x1, double y1, double x2, double y2);
    int FunGetAngleBetweenYlineAndIt(float x, float y);
    void FunSplitCommondBySpace(QString nowStr, QList<QString> &splitStrVec);

    bool FunGetMotion(QList<QString> &splitStrVec, int &nowMotion, int &allMotionCount);

    bool FunAnalyseValueStr(QString valueStr);

    bool Fun00Motion(QList<QString> &splitStrVec, int allMotionCount);
    bool Fun01Motion(QList<QString> &splitStrVec, int allMotionCount, QList<struShapeConf> &retShapeConf);
    bool Fun02Motion(QList<QString> &splitStrVec, int allMotionCount, QList<struShapeConf> &retShapeConf);
    bool Fun03Motion(QList<QString> &splitStrVec, int allMotionCount, QList<struShapeConf> &retShapeConf);
    void Fun17Motion(void);
    void Fun28Motion(void);
    void Fun90Motion(void);
    void Fun91Motion(void);

signals:

public slots:
};

#endif // CLAGCODEANALYSE_H
