#ifndef DIALOGCUTTINGSHAPE_H
#define DIALOGCUTTINGSHAPE_H

#include <QDialog>
//Self
#include "clacuttingscene.h"
#include "clashapeitem.h"

namespace Ui {
class DialogCuttingShape;
}

class DialogCuttingShape : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCuttingShape(QWidget *parent = 0);
    ~DialogCuttingShape();

    ClaCuttingScene *myClaScene;

    ClaShapeItem *myClaShapeItem;
    ClaPointReszie *tmpPoint;
    QPainterPath startPath;
    QList<QPainterPath> retPathsVec;

    void FunSetCuttingShapeConfig(int type, bool direct, int x, int y, int width, int high, int point, int angel, QColor color);

private:
    Ui::DialogCuttingShape *ui;
    void FunInitScene(void);
    void FunConnect(void);

    void FunBtnOk(void);

    void FunGetCuttingRange(int preX, int preY, int preW, int preH, int rangeX, int rangeY, int rangeW, int rangeH, QList<int> &cuttingRangeVec);
    void FunGetCuttingPath(int preX, int preY, int preW, int preH, QList<int> cuttingRangeVec, QList<struShapeConf> &newShapeVec);

signals:
    void SigCuttingFinished(QPainterPath retPathsVec, QList<struShapeConf> &newShapeVec);
};

#endif // DIALOGCUTTINGSHAPE_H
