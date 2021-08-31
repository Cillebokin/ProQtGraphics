#ifndef CLASHAPEOPERATE_H
#define CLASHAPEOPERATE_H

#include <QObject>
//Basic
#include <QListWidget>
//Self
#include "clashapeitem.h"
#include "clascene.h"

class ClaShapeOperate : public QObject
{
    Q_OBJECT
public:
    ClaShapeOperate(QObject *parent = 0);
    ~ClaShapeOperate();

    static void FunCopyShapeToClipboard(QList<QGraphicsItem *> itemList, QList<ClaShapeItem *> allGraphicsItems, QList<ClaShapeItem *> &allCopyItems);
    static void FunDeleteShapeBothVectorAndScene(QList<QGraphicsItem *> &itemList, QList<ClaShapeItem *> &allGraphicsItems, ClaScene* &myClaScene);
    static void FunClearShapeBothVectorAndScene(QList<ClaShapeItem *> &allGraphicsItems, ClaScene* &myClaScene);
    static void FunSetClockCommonStep(QList<QGraphicsItem *> itemList, int clockType);
    static QList<ClaShapeItem *> FunSetShapeRankCommonStep(QListWidget* &listWidge, QList<ClaShapeItem *> &allGraphicsItems);

private:


signals:

public slots:
};

#endif // CLASHAPEOPERATE_H
