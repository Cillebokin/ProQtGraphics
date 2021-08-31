#include "dialogcuttingshape.h"
#include "ui_dialogcuttingshape.h"

#include <QDebug>

DialogCuttingShape::DialogCuttingShape(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCuttingShape)
{
    ui->setupUi(this);

    FunInitScene();

    FunConnect();
}

DialogCuttingShape::~DialogCuttingShape()
{
    delete ui;
    delete myClaShapeItem;
    delete tmpPoint;
}

void DialogCuttingShape::FunInitScene(void)
{
    //Set myself graphics scene
    myClaScene = new ClaCuttingScene;
    myClaScene->setBackgroundBrush(Qt::white);

    //Add to graphics view
    ui->graphicsView->setScene(myClaScene);

    QPolygonF mypolygon1,mypolygon2;
    mypolygon1 << QPointF(0,25) << QPointF(50,25);
    mypolygon2 << QPointF(25,0) << QPointF(25,50);

    QPixmap pix(50,50);
    pix.fill(Qt::transparent);

    QPainter painter(&pix);

    QVector <qreal> dashes;
    dashes << 4 << 4 << 4 << 4;

    QPen pen(Qt::gray,0.5);
    pen.setDashPattern(dashes);
    painter.setPen(pen);
    painter.translate(0,0);
    painter.drawPolyline(mypolygon1);
    painter.drawPolyline(mypolygon2);

    myClaScene->setBackgroundBrush(pix);
}

void DialogCuttingShape::FunConnect(void)
{
    connect(ui->btnOk, &QPushButton::clicked, this, &DialogCuttingShape::FunBtnOk);
    connect(ui->btnCancel, &QPushButton::clicked, this, &DialogCuttingShape::close);
}

void DialogCuttingShape::FunBtnOk(void)
{
    qDebug() << myClaShapeItem->posX << "/" << myClaShapeItem->posY << " " << myClaShapeItem->rectWidth << "/" << myClaShapeItem->rectHigh;
    qDebug() << myClaScene->beginX << "/" << myClaScene->beginY << " " << myClaScene->endX << "/" << myClaScene->endY;

    QList<int> cuttingRangeVec;
    FunGetCuttingRange(myClaShapeItem->posX,
                       myClaShapeItem->posY,
                       myClaShapeItem->rectWidth,
                       myClaShapeItem->rectHigh,
                       myClaScene->beginX,
                       myClaScene->beginY,
                       myClaScene->endX,
                       myClaScene->endY,
                       cuttingRangeVec);

    QList<struShapeConf> newShapeVec;
    if(cuttingRangeVec.isEmpty())
    {
        //qDebug() << "裁剪范围不存在" << endl;
        DialogCuttingShape::close();
    }
    else
    {
        //qDebug() << cuttingRangeVec[0] << "/" << cuttingRangeVec[1] << " " << "w:" << cuttingRangeVec[2] << " h:" << cuttingRangeVec[3];
        FunGetCuttingPath(  myClaShapeItem->posX,
                            myClaShapeItem->posY,
                            myClaShapeItem->rectWidth,
                            myClaShapeItem->rectHigh,
                            cuttingRangeVec,
                            newShapeVec);
        emit SigCuttingFinished(startPath, newShapeVec);
        DialogCuttingShape::close();
    }
}

void DialogCuttingShape::FunSetCuttingShapeConfig(int type, bool direct, int x, int y, int width, int high, int point, int angel, QColor color)
{
    //Init new shape item
    myClaShapeItem = new ClaShapeItem;
    QPainterPath startPath = myClaShapeItem->FunSetItemConfig(type,direct,x,y,width,high,point,angel);

    QPen pen;
    pen.setColor(color);
    pen.setWidth(3);

    myClaShapeItem->setPen(pen);
    myClaShapeItem->setPath(startPath);

    myClaShapeItem->setFlag(QGraphicsItem::ItemIsMovable,false);
    myClaShapeItem->setFlag(QGraphicsItem::ItemIsSelectable,false);

    //Add item to allGraphicsItems and show it to scene
    myClaScene->addItem(myClaShapeItem);

}

void DialogCuttingShape::FunGetCuttingRange(int x1, int y1, int x2, int y2, int xx1, int yy1, int xx2, int yy2, QList<int> &cuttingRangeVec)
{
    //调整位置 1在左上角，2在右下角
    int temp;
    if (x1>x2)  { temp = x1, x1 = x2, x2 = temp; }
    if (y1>y2)  { temp = y1, y1 = y2, y2 = temp; }
    if (xx1>xx2){ temp = xx1, xx1 = xx2, xx2 = temp; }
    if (yy1>yy2){ temp = yy1; yy1 = yy2, yy2 = temp; }

    //第一个矩形在左，第二个矩形在右面
    if (x1>xx1)
    {
        temp = x1, x1 = xx1, xx1 = temp, temp = y1, y1 = yy1, yy1 = temp;
        temp = x2, x2 = xx2, xx2 = temp, temp = y2, y2 = yy2, yy2 = temp;
    }

    if ((xx1 >= x2) || yy1 >= y2 || y1>yy2){//相离
        int tx1 = xx1<x1 ? xx1 : x1, tx2 = xx2>x2 ? xx2 : x2, ty1 = y1<yy1 ? y1 : yy1, ty2 = y2>yy2 ? y2 : yy2;
        //qDebug() << "裁剪范围不存在" << endl;
        return;
    }
    else
    {  //相交（包含、部分相交）
        int tx1 = (x1>xx1 ? x1 : xx1), ty1 = (y1>yy1 ? y1 : yy1), tx2 = (x2<xx2 ? x2 : xx2), ty2 = (y2<yy2 ? y2 : yy2), l = tx2 - tx1, w = ty2 - ty1;
        //qDebug() << "裁剪范围:" << tx1 << ',' << ty1 << ',' << l << ',' << w << endl;
        cuttingRangeVec.push_back(tx1);
        cuttingRangeVec.push_back(ty1);
        cuttingRangeVec.push_back(l);
        cuttingRangeVec.push_back(w);
        return;
    }
}

void DialogCuttingShape::FunGetCuttingPath(int preX, int preY, int preW, int preH, QList<int> cuttingRangeVec, QList<struShapeConf> &newShapeVec)
{
    struShapeConf tmpConf1, tmpConf2, tmpConf3, tmpConf4;

    int xLeftUp = cuttingRangeVec[0], yLeftUp = cuttingRangeVec[1];
    int xRightUp = cuttingRangeVec[0] + cuttingRangeVec[2], yRightUp = cuttingRangeVec[1];
    int xRightDown = cuttingRangeVec[0] + cuttingRangeVec[2], yRightDown = cuttingRangeVec[1] + cuttingRangeVec[3];
    int xLeftDown = cuttingRangeVec[0], yLeftDown = cuttingRangeVec[1] + cuttingRangeVec[3];

    if(yLeftUp == preY && yRightUp == preY)
    {
        tmpConf1.typeConf = 0;
        tmpConf1.directConf = 0;

        tmpConf1.xConf = xLeftUp;
        tmpConf1.yConf = yLeftUp;
        tmpConf1.widthConf = xRightUp;
        tmpConf1.highConf = yLeftUp;

        tmpConf1.pointConf = 0;
        tmpConf1.angleConf = 0;

        newShapeVec.push_back(tmpConf1);
    }

    if(xRightUp == preX + preW && xRightDown == preX + preW)
    {
        tmpConf2.typeConf = 0;
        tmpConf2.directConf = 0;

        tmpConf2.xConf = xRightUp;
        tmpConf2.yConf = yRightUp;
        tmpConf2.widthConf = xRightDown;
        tmpConf2.highConf = yRightDown;

        tmpConf2.pointConf = 0;
        tmpConf2.angleConf = 0;

        newShapeVec.push_back(tmpConf2);
    }

    if(yRightDown == preY + preH && yLeftDown == preY + preH)
    {
        tmpConf3.typeConf = 0;
        tmpConf3.directConf = 0;

        tmpConf3.xConf = xRightDown;
        tmpConf3.yConf = yRightDown;
        tmpConf3.widthConf = xLeftDown;
        tmpConf3.highConf = yLeftDown;

        tmpConf3.pointConf = 0;
        tmpConf3.angleConf = 0;

        newShapeVec.push_back(tmpConf3);
    }

    if(xLeftDown == preX && xLeftUp == preX)
    {
        tmpConf4.typeConf = 0;
        tmpConf4.directConf = 0;

        tmpConf4.xConf = xLeftDown;
        tmpConf4.yConf = yLeftDown;
        tmpConf4.widthConf = xLeftUp;
        tmpConf4.highConf = yLeftUp;

        tmpConf4.pointConf = 0;
        tmpConf4.angleConf = 0;

        newShapeVec.push_back(tmpConf4);
    }
}
