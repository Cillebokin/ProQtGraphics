#include "clashapeoperate.h"
#include <QDebug>

#pragma region ClaShapeOperate{
ClaShapeOperate::ClaShapeOperate(QObject *parent) : QObject(parent)
{

}

ClaShapeOperate::~ClaShapeOperate()
{

}

void ClaShapeOperate::FunCopyShapeToClipboard(QList<QGraphicsItem *> itemList, QList<ClaShapeItem *> allGraphicsItems, QList<ClaShapeItem *> &allCopyItems)
{
    allCopyItems.clear();
    for(int i = 0; i < itemList.size(); i++)
    {
        for(int j = 0; j < allGraphicsItems.size(); j++)
        {
            if(allGraphicsItems[j] == itemList[i])
                allCopyItems.push_back(allGraphicsItems[j]);
        }
    }
}

void ClaShapeOperate::FunDeleteShapeBothVectorAndScene(QList<QGraphicsItem *> &itemList, QList<ClaShapeItem *> &allGraphicsItems, ClaScene* &myClaScene)
{
    for (int i = 0; i < itemList.size(); i++)
    {
        if(0 != itemList[i]->childItems().size())
        {
            allGraphicsItems.removeOne((ClaShapeItem *)itemList[i]);
            myClaScene->removeItem(itemList[i]);
            delete itemList[i];
        }
    }
}

void ClaShapeOperate::FunClearShapeBothVectorAndScene(QList<ClaShapeItem *> &allGraphicsItems, ClaScene* &myClaScene)
{
    for(QList<ClaShapeItem *>::iterator iter = allGraphicsItems.begin(); iter != allGraphicsItems.end(); iter++)
    {
        allGraphicsItems.removeOne(*iter);
        myClaScene->removeItem(*iter);
        delete *iter;
    }
}

void ClaShapeOperate::FunSetClockCommonStep(QList<QGraphicsItem *> itemList, int clockType)
{
    for (int i = 0; i < itemList.size(); i++)
    {
        if(0 != itemList[i]->childItems().size())
        {
            ClaShapeItem *tmpItem = static_cast<ClaShapeItem *>(itemList[i]);
            tmpItem->FunSetMakeDirect(clockType);
        }
    }
}

QList<ClaShapeItem *> ClaShapeOperate::FunSetShapeRankCommonStep(QListWidget* &listWidget, QList<ClaShapeItem *> &allGraphicsItems)
{
    QList<ClaShapeItem *> newAllGraphicsItems;
    for(int i = 0; i < listWidget->count(); i++)
    {
        int num = listWidget->item(i)->text().left(listWidget->item(i)->text().indexOf(':')).toInt();
        newAllGraphicsItems.push_back(allGraphicsItems[num - 1]);
    }

    return newAllGraphicsItems;
}
#pragma endregion}
