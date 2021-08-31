#include "clacuttingscene.h"
#include <QDebug>

ClaCuttingScene::ClaCuttingScene(QObject *parent) : QGraphicsScene(parent)
{
    beginX = 0;
    beginY = 0;
    endX = 0;
    endY = 0;

    myQGraphicsPathItem = new QGraphicsPathItem;
}

ClaCuttingScene::~ClaCuttingScene()
{

}

void ClaCuttingScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //qDebug() << "MoveEvent:" << mouseEvent->scenePos().x() << "/" << mouseEvent->scenePos().y();
}

void ClaCuttingScene::mousePressEvent  (QGraphicsSceneMouseEvent * mouseEvent )
{
    //qDebug() << "PressEvent:" << mouseEvent->scenePos().x() << "/" << mouseEvent->scenePos().y();
    beginX = mouseEvent->scenePos().x();
    beginY = mouseEvent->scenePos().y();

    delete myQGraphicsPathItem;
}

void ClaCuttingScene::mouseReleaseEvent  (QGraphicsSceneMouseEvent * mouseEvent )
{
    endX = mouseEvent->scenePos().x();
    endY = mouseEvent->scenePos().y();

    QPainterPath startPath;
    startPath.moveTo(beginX, beginY);
    startPath.lineTo(beginX, endY);
    startPath.lineTo(endX, endY);
    startPath.lineTo(endX, beginY);
    startPath.lineTo(beginX, beginY);

    QPen pen;
    pen.setColor(Qt::black);
    pen.setStyle(Qt::DashLine);
    pen.setWidth(1.5);

    myQGraphicsPathItem = new QGraphicsPathItem;
    myQGraphicsPathItem->setPen(pen);
    myQGraphicsPathItem->setPath(startPath);

    this->addItem(myQGraphicsPathItem);
}

