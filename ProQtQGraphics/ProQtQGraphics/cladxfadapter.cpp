#include "cladxfadapter.h"
#include <QDebug>

ClaDxfAdapter::ClaDxfAdapter()
{

}

ClaDxfAdapter::~ClaDxfAdapter()
{

}

void ClaDxfAdapter::addLine(const DL_LineData& d)
{
    qDebug() << "Line: " << d.x1 << "/" << d.y1 << " " << d.x2 << "/" << d.y2;

    struShapeConf tmpShapeConf;
    tmpShapeConf.typeConf = 0;
    tmpShapeConf.directConf = 0;
    tmpShapeConf.xConf = d.x1 / 5;
    tmpShapeConf.yConf = d.y1 / 5;
    tmpShapeConf.widthConf = d.x2 / 5;
    tmpShapeConf.highConf = d.y2 / 5;
    tmpShapeConf.pointConf = 0;
    tmpShapeConf.angleConf = 0;

    shapeConfVec.push_back(tmpShapeConf);
}

void ClaDxfAdapter::addArc(const DL_ArcData& d)
{
    qDebug() << "Arc: " << d.cx << "/" << d.cy << "/" << d.angle1 << "/" << d.angle2;

    struShapeConf tmpShapeConf;
    tmpShapeConf.typeConf = 2;
    tmpShapeConf.directConf = 0;
    tmpShapeConf.xConf = d.cx / 5 - d.radius / 5;
    tmpShapeConf.yConf = d.cy / 5 - d.radius / 5;
    tmpShapeConf.widthConf = d.radius / 5 * 2;
    tmpShapeConf.highConf = d.radius / 5 *2;
    tmpShapeConf.pointConf = d.angle1;
    tmpShapeConf.angleConf = 360 - d.angle1 + d.angle2;

    shapeConfVec.push_back(tmpShapeConf);
}
void ClaDxfAdapter::addCircle(const DL_CircleData& d)
{
    qDebug() << "Circle:" << d.cx << "/" << d.cy << "/" << d.radius;

    struShapeConf tmpShapeConf;
    tmpShapeConf.typeConf = 3;
    tmpShapeConf.directConf = 0;
    tmpShapeConf.xConf = d.cx / 5 - d.radius / 5;
    tmpShapeConf.yConf = d.cy / 5 - d.radius / 5;
    tmpShapeConf.widthConf = d.radius / 5 * 2;
    tmpShapeConf.highConf = d.radius / 5 *2;
    tmpShapeConf.pointConf = 0;
    tmpShapeConf.angleConf = 360;

    shapeConfVec.push_back(tmpShapeConf);
}
