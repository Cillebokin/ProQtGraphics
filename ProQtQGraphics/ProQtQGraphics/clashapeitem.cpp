#include "clashapeitem.h"
#include <QDebug>
#include <QPen>
#include <QMenu>
#include <QInputDialog>

#include "dialogcuttingshape.h"

#pragma region ClaShapeItem{
ClaShapeItem::ClaShapeItem(QObject *parent) : QObject(parent)
{
    connect(this, &ClaShapeItem::SigSetNewWidthHigh, this, &ClaShapeItem::SloPointMoved);
}
ClaShapeItem::~ClaShapeItem()
{

}

QPainterPath ClaShapeItem::FunSetItemConfig(int type, bool direct, int x, int y, int width, int high, int point, int angel)
{
    addrName = (int)(this);

    shapeType = type;
    makeDirect = direct;
    posX = x;
    posY = y;
    rectWidth = width;
    rectHigh = high;
    beginPoint = point;
    arcAngle = angel;

    QPainterPath startPath;
    FunGetRealPath(startPath);

    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);

    return startPath;
}

void ClaShapeItem::FunGetRealPath(QPainterPath &startPath)
{
    switch(shapeType)
    {
    case TypeLine:
        startPath.moveTo(posX,posY);
        startPath.lineTo(rectWidth,rectHigh);
        break;

    case TypeRect:
        startPath.moveTo(posX,posY);
        startPath.lineTo(rectWidth,posY);
        startPath.lineTo(rectWidth,rectHigh);
        startPath.lineTo(posX,rectHigh);
        startPath.lineTo(posX,posY);
        break;

    case TypeArc:
        startPath.arcMoveTo(posX,posY,rectWidth,rectHigh,beginPoint+90);
        startPath.arcTo(posX,posY,rectWidth,rectHigh,beginPoint+90,arcAngle);
        break;

    case TypeCircle:
        startPath.arcMoveTo(posX,posY,rectWidth,rectHigh,0);
        startPath.arcTo(posX,posY,rectWidth,rectHigh,0,360);
        break;
    }
}

void ClaShapeItem::FunSetMakeDirect(bool newDirect)
{
    makeDirect = newDirect;
}

void ClaShapeItem::FunInitControllerForMenu(QMenu* &menu, QLabel* &labOrder, QSpinBox* &spinRectWidth, QSpinBox* &spinRectHigh, QSpinBox* &spinRotate, QLabel* &labNowDirect, QPushButton* &btnRedirect, QPushButton* &btnCutting)
{
    labOrder = new QLabel("Addr: " + QString::number((int)(this),16),menu);
    labOrder->setFont(QFont("Arial", 10, 150));
    labOrder->setStyleSheet("color:blue");

    spinRectWidth = new QSpinBox(menu);
    spinRectWidth->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    spinRectWidth->setRange(0, 2000);
    spinRectWidth->setPrefix("Width: ");
    spinRectWidth->setSingleStep(1);
    spinRectWidth->setValue(this->rectWidth);
    connect(spinRectWidth, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int nowWidth){
            emit SigSetNewWidthHigh(nowWidth, this->rectHigh);
        });

    spinRectHigh = new QSpinBox(menu);
    spinRectHigh->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    spinRectHigh->setRange(0, 2000);
    spinRectHigh->setPrefix("High : ");
    spinRectHigh->setSingleStep(1);
    spinRectHigh->setValue(this->rectHigh);
    connect(spinRectHigh, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int nowHigh){
            emit SigSetNewWidthHigh(this->rectWidth, nowHigh);
        });

    spinRotate = new QSpinBox(menu);
    spinRotate->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    spinRotate->setRange(-180, 180);
    spinRotate->setPrefix("Rotate: ");
    spinRotate->setSingleStep(1);
    spinRotate->setValue(this->rotation());
    connect(spinRotate, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int rotateAnagle){
            this->setRotation(rotateAnagle);
        });

    labNowDirect = new QLabel(menu);
    labNowDirect->setScaledContents(true);
    labNowDirect->setAlignment(Qt::AlignHCenter);
    labNowDirect->setAlignment(Qt::AlignVCenter);
    if(0 == this->makeDirect)
        labNowDirect->setPixmap(QPixmap(":/new/pictrues/pictrues/Clockwise.png"));
    else
        labNowDirect->setPixmap(QPixmap(":/new/pictrues/pictrues/AntiClockwise.png"));

    btnRedirect = new QPushButton("Redirect", menu);
    connect(btnRedirect, &QPushButton::clicked, [=](void){
        this->makeDirect = !(this->makeDirect);
        if(0 == this->makeDirect)
            labNowDirect->setPixmap(QPixmap(":/new/pictrues/pictrues/Clockwise.png"));
        else
            labNowDirect->setPixmap(QPixmap(":/new/pictrues/pictrues/AntiClockwise.png"));
    });

    btnCutting = new QPushButton("Cutting", menu);
    connect(btnCutting, &QPushButton::clicked, [=](void){
        if(TypeArc == this->shapeType || TypeCircle == this->shapeType)
        {
            bool ok;
            int i = QInputDialog::getInt(nullptr, tr("beginPoint"), tr("Input beginPoint:"), 0, -360, 360, 1, &ok);
            int j = QInputDialog::getInt(nullptr, tr("arcAngle"), tr("Input arcAngle:"), 90, -360, 360, 1, &ok);

            QPainterPath arcPath = this->FunSetItemConfig(TypeArc,
                                                          this->makeDirect,
                                                          this->scenePos().x(),
                                                          this->scenePos().y(),
                                                          this->rectWidth,
                                                          this->rectHigh,
                                                          i,
                                                          j);
            this->setPath(arcPath);

            ClaPointReszie* tmpResizePoint = static_cast<ClaPointReszie*>(this->childItems().at(0));
            QPainterPath tmpResizePointPath;
            tmpResizePointPath.moveTo(this->scenePos().x()+this->rectWidth,this->scenePos().y()+this->rectHigh);
            tmpResizePointPath.lineTo(this->scenePos().x()+this->rectWidth + 5,this->scenePos().y()+this->rectHigh);
            tmpResizePointPath.lineTo(this->scenePos().x()+this->rectWidth + 5,this->scenePos().y()+this->rectHigh + 5);
            tmpResizePointPath.lineTo(this->scenePos().x()+this->rectWidth,this->scenePos().y()+this->rectHigh + 5);
            tmpResizePointPath.lineTo(this->scenePos().x()+this->rectWidth,this->scenePos().y()+this->rectHigh);

            tmpResizePoint->setPath(tmpResizePointPath);
        }
        else
        {
            DialogCuttingShape *myDialogCuttingShape;
            myDialogCuttingShape = new DialogCuttingShape;
            myDialogCuttingShape->setWindowTitle("Cutting");
            myDialogCuttingShape->setFixedSize(this->rectWidth + 200,this->rectHigh + 200);
            myDialogCuttingShape->FunSetCuttingShapeConfig(shapeType, makeDirect, posX, posY, rectWidth, rectHigh, beginPoint, arcAngle, Qt::black);
            myDialogCuttingShape->show();
            connect(myDialogCuttingShape, &DialogCuttingShape::SigCuttingFinished, this, &ClaShapeItem::SloCuttingFinished);
        }
    });
}

void ClaShapeItem::FunAddWidgetActionToMenu(QMenu* &menu, QWidgetAction *wdgactLabOrder, QWidgetAction *wdgactSpinRectWidth, QWidgetAction *wdgactSpinRectHigh, QWidgetAction *wdgactSpinRotate, QWidgetAction *wdgactLabNowDirect, QWidgetAction *wdgactBtnCopy, QWidgetAction *wdgactBtnCutting)
{
    menu->addAction(wdgactLabOrder);
    menu->addSeparator();

    menu->addAction(wdgactSpinRectWidth);
    menu->addAction(wdgactSpinRectHigh);
    menu->addSeparator();

    menu->addAction(wdgactSpinRotate);

    menu->addSeparator();
    menu->addAction(wdgactLabNowDirect);
    menu->addAction(wdgactBtnCopy);

    if(TypeRect == this->shapeType || TypeCircle == this->shapeType || TypeArc == this->shapeType)
    {
        menu->addSeparator();
        menu->addAction(wdgactBtnCutting);
    }
}

void ClaShapeItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if (!this->isSelected())
        return;

    this->setSelected(true);

    QMenu* menu = new QMenu();
    menu->setStyleSheet("QMenu { background-color:rgb(255,255,255); border: 1px solid rgb(0,0,0); }");

    QLabel *labOrder;
    QSpinBox *spinRectWidth;
    QSpinBox *spinRectHigh;
    QSpinBox *spinRotate;
    QLabel *labNowDirect;
    QPushButton *btnRedirect;
    QPushButton *btnCutting;
    FunInitControllerForMenu(menu,labOrder,spinRectWidth,spinRectHigh,spinRotate,labNowDirect,btnRedirect,btnCutting);

    QWidgetAction *wdgactLabOrder = new QWidgetAction(menu);
    wdgactLabOrder->setDefaultWidget(labOrder);

    QWidgetAction *wdgactSpinRectWidth = new QWidgetAction(menu);
    wdgactSpinRectWidth->setDefaultWidget(spinRectWidth);
    QWidgetAction *wdgactSpinRectHigh = new QWidgetAction(menu);
    wdgactSpinRectHigh->setDefaultWidget(spinRectHigh);

    QWidgetAction *wdgactSpinRotate = new QWidgetAction(menu);
    wdgactSpinRotate->setDefaultWidget(spinRotate);

    QWidgetAction *wdgactLabNowDirect = new QWidgetAction(menu);
    wdgactLabNowDirect->setDefaultWidget(labNowDirect);

    QWidgetAction *wdgactBtnCopy = new QWidgetAction(menu);
    wdgactBtnCopy->setDefaultWidget(btnRedirect);

    QWidgetAction *wdgactBtnCutting = new QWidgetAction(menu);
    wdgactBtnCutting->setDefaultWidget(btnCutting);

    FunAddWidgetActionToMenu(menu,wdgactLabOrder,wdgactSpinRectWidth,wdgactSpinRectHigh,wdgactSpinRotate,wdgactLabNowDirect,wdgactBtnCopy,wdgactBtnCutting);

    menu->exec(QCursor::pos());
    delete menu;

    QGraphicsItem::contextMenuEvent(event);
}

void ClaShapeItem::SloPointMoved(int x, int y)
{
    QPainterPath startPath = this->FunSetItemConfig(shapeType,makeDirect,posX,posY,x,y,beginPoint,arcAngle);
    this->setPath(startPath);
}

void ClaShapeItem::SloCuttingFinished(QPainterPath cuttingPath, QList<struShapeConf> &newShapeVec)
{
    //this->setPath(cuttingPath);

    SigNewCuttingShape(newShapeVec);
}

void ClaShapeItem::focusInEvent(QFocusEvent *focusEvent)
{
    Q_UNUSED(focusEvent);
    emit SigSelectedItemList(this->addrName);
}

void ClaShapeItem::focusOutEvent(QFocusEvent *focusEvent)
{
    Q_UNUSED(focusEvent);
    //emit SigSelectedItemList();
}

void ClaShapeItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsPathItem::mouseMoveEvent(event);
}
#pragma endregion}

#pragma region ClaPointReszie{
ClaPointReszie::ClaPointReszie(QGraphicsPathItem *parent) : QGraphicsPathItem(parent)
{

}
ClaPointReszie::~ClaPointReszie()
{

}

void ClaPointReszie::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    int x = event->scenePos().x() - this->parentItem()->pos().x();
    int y = event->scenePos().y() - this->parentItem()->pos().y();

    QPainterPath startPath;
    startPath.moveTo(x,y);
    startPath.lineTo(x + 5,y);
    startPath.lineTo(x + 5,y + 5);
    startPath.lineTo(x,y + 5);
    startPath.lineTo(x,y);

    this->setPath(startPath);

    emit SigPointMoved(x, y);
}

QPainterPath ClaPointReszie::FunInitPointResize(int posX, int posY)
{
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(0.1);

    this->setBrush(Qt::cyan);
    this->setPen(pen);

    QPainterPath startPath;
    startPath.moveTo(posX,posY);
    startPath.lineTo(posX + 5,posY);
    startPath.lineTo(posX + 5,posY + 5);
    startPath.lineTo(posX,posY + 5);
    startPath.lineTo(posX,posY);

    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);

    return startPath;
}

void ClaPointReszie::SloPointMoved(int x, int y)
{
    QPainterPath startPath;
    startPath.moveTo(x,y);
    startPath.lineTo(x + 5,y);
    startPath.lineTo(x + 5,y + 5);
    startPath.lineTo(x,y + 5);
    startPath.lineTo(x,y);

    this->setPath(startPath);
}
#pragma endregion}
