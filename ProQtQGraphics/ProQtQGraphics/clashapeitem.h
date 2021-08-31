#ifndef CLASHAPEITEM_H
#define CLASHAPEITEM_H

#include <QObject>
//Basic
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QWidgetAction>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneDragDropEvent>

typedef struct
{
    int typeConf;
    bool directConf;
    int xConf;
    int yConf;
    int widthConf;
    int highConf;
    int pointConf;
    int angleConf;
}struShapeConf;

typedef struct
{
    short lineNum;
    short moveType;//0:ptp 1:line 2:circle 200:end

    double targetX;
    double targetY;
    double targetZ;

    double supportX;
    double supportY;
    double supportZ;
}struPlcLineConf;

class ClaPointReszie : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:
    ClaPointReszie(QGraphicsPathItem *parent = 0);
    ~ClaPointReszie();

    QPainterPath FunInitPointResize(int posX, int posY);

protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event)override;

signals:
    void SigPointMoved(int x, int y);

public slots:
    void SloPointMoved(int x, int y);
};

class ClaShapeItem : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:
    ClaShapeItem(QObject *parent = 0);
    ~ClaShapeItem();

    enum EnumTypeShape{TypeLine = 0, TypeRect = 1, TypeArc, TypeCircle, TypeIntersect};

    int addrName;

    int shapeType;
    bool makeDirect;

    int posX;
    int posY;

    int rectWidth;
    int rectHigh;

    int beginPoint;
    int arcAngle;

    QPainterPath FunSetItemConfig(int type, bool direct, int x, int y, int width, int high, int point, int angel);
    void FunSetMakeDirect(bool newDirect);

private:
    void FunGetRealPath(QPainterPath &startPath);
    void FunInitControllerForMenu(QMenu* &menu, QLabel* &labOrder, QSpinBox* &spinRectWidth, QSpinBox* &spinRectHigh, QSpinBox* &spinRotate, QLabel* &labNowDirect, QPushButton* &btnRedirect, QPushButton* &btnCutting);
    void FunAddWidgetActionToMenu(QMenu* &menu, QWidgetAction *wdgactLabOrder, QWidgetAction *wdgactSpinRectWidth, QWidgetAction *wdgactSpinRectHigh, QWidgetAction *wdgactSpinRotate, QWidgetAction *wdgactLabNowDirect, QWidgetAction *wdgactBtnCopy, QWidgetAction *wdgactBtnCutting);

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent * event)override;
    virtual void focusInEvent(QFocusEvent *focusEvent)override;
    virtual void focusOutEvent(QFocusEvent *focusEvent)override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event)override;

signals:
    void SigSelectedItemList(int);
    void SigSetNewWidthHigh(int newWidth, int newHigh);
    void SigNewCuttingShape(QList<struShapeConf> &newShapeVec);

public slots:
    void SloPointMoved(int x, int y);
    void SloCuttingFinished(QPainterPath cuttingPath, QList<struShapeConf> &newShapeVec);
};


#endif // CLASHAPEITEM_H
