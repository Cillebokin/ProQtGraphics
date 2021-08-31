#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//Self
#include "clascene.h"
#include "clashapeitem.h"
#include "clashapeoperate.h"
#include "clagcodeanalyse.h"
#include "clatimer.h"
#include "cladrawbyplc.h"
#include "dialogshowwaveline.h"

#include "x_net.h"
#include "cladxfadapter.h"
#include "threadgetxy.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    short numOn = 256;
    short numOff = 0;
    enum EnumTypeShape{TypeLine = 0, TypeRect = 1, TypeArc, TypeCircle, TypeIntersect};
    enum EnumClockType{EnumClockwise = 0, EnumAntiClockwise};

private:
    //All Shape Items
    QList<ClaShapeItem *> allGraphicsItems;
    //Clip Board
    QList<ClaShapeItem *> allCopyItems;
    //Gcode Analyse Config
    //QList<struShapeConf> allAnalyConf;
    //PLC Line Conf
    QList<struPlcLineConf> plcLineConfVec;

    //Window Menu
    QAction *actOpenFile;
    QAction *actSaveFile;
    QAction *actCloseWindow;

    QAction *actCopyShape;
    QAction *actPasteShape;

    QAction *actDeleteShape;
    QAction *actClearShape;

    QAction *actShowWaveLine;

    QAction *actPenColor;
    QAction *actClockwise;
    QAction *actAnticlockwise;

    QAction *actAbout;

    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuTool;
    QMenu *menuHelp;

    //Window Toolbar
    QAction *actCreLine;
    QAction *actCreArc;
    QAction *actCreArcHoriz;
    QAction *actCreArcVerti;
    QAction *actCreRect;
    QAction *actCreCircle;

    QAction *actSetRank;

    QAction *actAllShowMess;

    //Graphics
    QPen pen;
    ClaScene *myClaScene;

    //X_Net
    X_Net *service;
    ClaDrawByPlc *myClaDrawByPlc;

private:
    Ui::MainWindow *ui;

    //INIT UI
    void FunInitUi(void);
    void FunInitMenuBar(void);
    void FunInitToolBar(void);
    void FunInitStatusBar(void);
    void FunInitScene(void);
    void FunShowAboutMessagebox(void);

    //SET CONNECT
    void FunConnect(void);

    //DRAW SHAPE
    void FunDrawShapeCommonStep(int type, bool direct, int x, int y, int width, int high, int point, int angel);
    void FunDrawShapeCommonStepForCutting(int type, bool direct, int x, int y, int width, int high, int point, int angel);

    void FunDrawLineOnScene(void);
    void FunDrawRectOnScene(void);
    void FunDrawArcOnScene(void);
    void FunDrawArcHorizOnScene(void);
    void FunDrawArcVertiOnScene(void);
    void FunDrawCircleOnScene(void);

    //SHAPE OPERATE
    void FunOpenFile(void);
    void FunSaveFile(void);

    void FunCopyShape(void);
    void FunPasteShape(void);

    void FunSetColorOfPen(void);

    void FunDeleteOneShape(void);
    void FunClearAllShapes(void);

    void FunSetClockwise(void);
    void FunSetAnticlockwise(void);

    void FunSetShapeRank(void);
    void FunAllShowMess(void);

    //WAVE LINE
    void FunShowWaveLine(void);

    //PLC
    void FunGetRealPositionOfPreDraw(ClaShapeItem *nowPreDraw, int &realBeginX, int &realBeginY, int &realEndX, int &realEndY);
    void FunGetRealPositionOfPreLine(ClaShapeItem *nowPreLine, int &realBeginX, int &realBeginY, int &realEndX, int &realEndY);
    void FunBtnClickedConnectPlc(void);
    void FunBtnClickedMotorEnable(void);

    void FunBtnClickedWorkBeginStop(void);
    void FunBtnClickedWorkHaltContinue(void);

    void FunGetPositionByAngle(int angle, int centerX, int centerY, int r, double &realX, double &realY);
    void FunGetAllShapeMovePathes(void);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

signals:
    void SigRenewItemList(void);

public slots:
    void SloRenewItemList(void);
    void SloSelectedItemList(int);
    void SloNewCuttingShape(QList<struShapeConf> &newShapeVec);
};

#endif // MAINWINDOW_H
