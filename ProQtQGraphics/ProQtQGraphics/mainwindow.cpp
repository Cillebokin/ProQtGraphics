#include "mainwindow.h"
#include "ui_mainwindow.h"

//Basic
#include <QDebug>
#include <QMessageBox>
//Operator
#include <QFileDialog>
#include <QColorDialog>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    FunInitUi();
    FunInitMenuBar();
    FunInitToolBar();
    FunInitStatusBar();
    FunInitScene();

    FunConnect();
}
MainWindow::~MainWindow()
{
    delete ui;
    delete myClaScene;
}

#pragma region InitUi{
void MainWindow::FunInitUi(void)
{
    //Widget
    this->resize(1349,938);
    this->setWindowTitle("QGraphics Test");

    //Set config of mouse
    ui->centralWidget->setMouseTracking(true);
    this->setMouseTracking(true);

    //Set config of listWidget
    ui->listWidget->setDragEnabled(true);
    ui->listWidget->setAcceptDrops(true);
    ui->listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listWidget->setStyleSheet("color:gary");

    //Set config of commmon pen
    pen.setColor(QColor(0,0,0));
    pen.setWidth(3);

    //Set PLC IP Address
    ui->lnedPlcIp->setPlaceholderText("192.168.6.6");
    QRegExp rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
    QRegExpValidator *m_IPValidator = new QRegExpValidator(rx, this);
    ui->lnedPlcIp->setValidator(m_IPValidator);

    ui->btnMakeSpinCan->setEnabled(false);

    ui->btnWorkBeginStop->setEnabled(false);
    ui->btnWorkHaltContinue->setEnabled(false);

    //XNet
    myClaDrawByPlc = new ClaDrawByPlc;
    service = new X_Net;
}
void MainWindow::FunInitMenuBar(void)
{
    //MenuFile action
    actOpenFile = new QAction(tr("Open File"),this);
    actOpenFile->setShortcut(QKeySequence(tr("ctrl+o")));
    actSaveFile = new QAction(tr("Save File"),this);
    actSaveFile->setShortcut(QKeySequence(tr("ctrl+s")));
    actCloseWindow = new QAction(tr("Close Window"),this);
    actCloseWindow->setShortcut(QKeySequence(tr("ctrl+q")));

    //MenuEdit action
    actCopyShape = new QAction(tr("Copy Item"),this);
    actCopyShape->setShortcut(QKeySequence(tr("ctrl+c")));
    actPasteShape = new QAction(tr("Paste Item"),this);
    actPasteShape->setShortcut(QKeySequence(tr("ctrl+v")));
    actDeleteShape = new QAction(tr("Delete Item"),this);
    actDeleteShape->setShortcut(QKeySequence(tr("delete")));
    actClearShape = new QAction(tr("Clear All Items"),this);
    actClearShape->setShortcut(QKeySequence(tr("ctrl+delete")));

    //MenuTool action
    actPenColor = new QAction(tr("Set Pen Color"),this);
    actPenColor->setShortcut(QKeySequence(tr("ctrl+p")));
    actClockwise = new QAction(tr("Set Clockwise"),this);
    actClockwise->setShortcut(QKeySequence(tr("ctrl+d")));
    actAnticlockwise = new QAction(tr("Set Anticlockwise"),this);
    actAnticlockwise->setShortcut(QKeySequence(tr("ctrl+shift+d")));
    actShowWaveLine = new QAction(tr("Show Wave Line"),this);
    actShowWaveLine->setShortcut(QKeySequence(tr("ctrl+shift+w")));

    //MenuHelp action
    actAbout = new QAction(tr("About this software"),this);

    //Set MenuFile
    menuFile = new QMenu;
    menuFile = menuBar()->addMenu(tr("File"));
    menuFile->addAction(actOpenFile);
    menuFile->addAction(actSaveFile);
    menuFile->addSeparator();
    menuFile->addAction(actCloseWindow);

    //Set MenuEdit
    menuEdit = new QMenu;
    menuEdit = menuBar()->addMenu(tr("Edit"));
    menuEdit->addAction(actCopyShape);
    menuEdit->addAction(actPasteShape);
    menuEdit->addSeparator();
    menuEdit->addAction(actDeleteShape);
    menuEdit->addAction(actClearShape);

    //Set MenuTool
    menuTool = new QMenu;
    menuTool = menuBar()->addMenu(tr("Tool"));
    menuTool->addAction(actPenColor);
    menuTool->addSeparator();
    menuTool->addAction(actClockwise);
    menuTool->addAction(actAnticlockwise);
    menuTool->addSeparator();
    menuTool->addAction(actShowWaveLine);

    //Set MenuHelp
    menuHelp = new QMenu;
    menuHelp = menuBar()->addMenu(tr("Help"));
    menuHelp->addAction(actAbout);
}
void MainWindow::FunInitToolBar(void)
{
    ui->mainToolBar->setMovable(false);

    //ToolBar action
    actCreLine = new QAction(tr("Line"),this);
    actCreArc = new QAction(tr("Arc"),this);
    actCreArcHoriz = new QAction(tr("ArcHoriz"),this);
    actCreArcVerti = new QAction(tr("ArcVerti"),this);
    actCreRect = new QAction(tr("Rect"),this);
    actCreCircle = new QAction(tr("Circle"),this);

    actSetRank = new QAction(tr("Rank"),this);

    actAllShowMess = new QAction(tr("Show"),this);

    //Add action to ToolBar
    ui->mainToolBar->addAction(actCreLine);
    ui->mainToolBar->addAction(actCreArc);
    ui->mainToolBar->addAction(actCreArcHoriz);
    ui->mainToolBar->addAction(actCreArcVerti);
    ui->mainToolBar->addAction(actCreRect);
    ui->mainToolBar->addAction(actCreCircle);

    ui->mainToolBar->addSeparator();

    ui->mainToolBar->addAction(actSetRank);
    ui->mainToolBar->addAction(actAllShowMess);
}
void MainWindow::FunInitStatusBar(void)
{
    ui->statusBar->setSizeGripEnabled(false);
}
void MainWindow::FunInitScene(void)
{
    //Set myself graphics scene
    myClaScene = new ClaScene;
    myClaScene->setBackgroundBrush(Qt::white);
    //myClaScene->setSceneRect(0,0,ui->graphicsView->width(), ui->graphicsView->height());

    //Add to graphics view
    ui->graphicsView->setScene(myClaScene);
}
void MainWindow::FunShowAboutMessagebox(void)
{
    //Set message box for help action
    QMessageBox::about(NULL, "About", "QGraphics Test ver0.1\n\tby Li Zhihong");
}
void MainWindow::resizeEvent(QResizeEvent *event)
{
    //Set grid line for graphics scene when the window size changes
    Q_UNUSED(event);

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
#pragma endregion}

#pragma region SetConnect{
void MainWindow::FunConnect(void)
{
    //MenuFile action
    connect(actOpenFile, &QAction::triggered, this, &MainWindow::FunOpenFile);
    connect(actSaveFile, &QAction::triggered, this, &MainWindow::FunSaveFile);
    connect(actCloseWindow, &QAction::triggered, this, &MainWindow::close);
    //MenuEdit action
    connect(actCopyShape, &QAction::triggered, this, &MainWindow::FunCopyShape);
    connect(actPasteShape, &QAction::triggered, this, &MainWindow::FunPasteShape);
    connect(actAbout, &QAction::triggered, this, &MainWindow::FunShowAboutMessagebox);
    connect(actDeleteShape, &QAction::triggered, this, &MainWindow::FunDeleteOneShape);
    connect(actClearShape, &QAction::triggered, this, &MainWindow::FunClearAllShapes);
    //MenuTool action
    connect(actPenColor, &QAction::triggered, this, &MainWindow::FunSetColorOfPen);
    connect(actClockwise, &QAction::triggered, this, &MainWindow::FunSetClockwise);
    connect(actAnticlockwise, &QAction::triggered, this, &MainWindow::FunSetAnticlockwise);
    connect(actShowWaveLine, &QAction::triggered, this, &MainWindow::FunShowWaveLine);

    //Toolbar action
    connect(actCreLine, &QAction::triggered, this, &MainWindow::FunDrawLineOnScene);
    connect(actCreRect, &QAction::triggered, this, &MainWindow::FunDrawRectOnScene);
    connect(actCreArc, &QAction::triggered, this, &MainWindow::FunDrawArcOnScene);
    connect(actCreArcHoriz, &QAction::triggered, this, &MainWindow::FunDrawArcHorizOnScene);
    connect(actCreArcVerti, &QAction::triggered, this, &MainWindow::FunDrawArcVertiOnScene);
    connect(actCreCircle, &QAction::triggered, this, &MainWindow::FunDrawCircleOnScene);
    connect(actSetRank, &QAction::triggered, this, &MainWindow::FunSetShapeRank);
    connect(actAllShowMess, &QAction::triggered, this, &MainWindow::FunAllShowMess);

    //Another connect
    connect(this, &MainWindow::SigRenewItemList, this, &MainWindow::SloRenewItemList);

    //PLC
    connect(ui->btnConnetPlc, &QPushButton::clicked, this, &MainWindow::FunBtnClickedConnectPlc);
    connect(ui->btnMakeSpinCan, &QPushButton::clicked, this, &MainWindow::FunBtnClickedMotorEnable);
    connect(ui->btnWorkBeginStop, &QPushButton::clicked, this, &MainWindow::FunBtnClickedWorkBeginStop);
    connect(ui->btnWorkHaltContinue, &QPushButton::clicked, this, &MainWindow::FunBtnClickedWorkHaltContinue);
}
#pragma endregion}

#pragma region DrawShape{
void MainWindow::FunDrawShapeCommonStep(int type, bool direct, int x, int y, int width, int high, int point, int angel)
{
    //Init new shape item
    ClaShapeItem *tmpItem = new ClaShapeItem;
    QPainterPath startPath = tmpItem->FunSetItemConfig(type,direct,x,y,width,high,point,angel);
    tmpItem->setPen(pen);
    tmpItem->setPath(startPath);
    //Add item to allGraphicsItems and show it to scene
    myClaScene->addItem(tmpItem);
    allGraphicsItems.push_back(tmpItem);

    //Init a little point for resize
    ClaPointReszie *tmpPoint = new ClaPointReszie(tmpItem);
    QPainterPath startPathPoint = tmpPoint->FunInitPointResize(width,high);
    tmpPoint->setPath(startPathPoint);

    //ListWidget should choosed item when shape has choosed
    connect(tmpItem, &ClaShapeItem::SigSelectedItemList, this, &MainWindow::SloSelectedItemList);
    //Reset the new path for Shape when resize point moved
    connect(tmpPoint, &ClaPointReszie::SigPointMoved, tmpItem, &ClaShapeItem::SloPointMoved);
    //Set new width and high of shape item just should renew position of resize point
    connect(tmpItem, &ClaShapeItem::SigSetNewWidthHigh, tmpPoint, &ClaPointReszie::SloPointMoved);
    //Cutting new shape items
    connect(tmpItem, &ClaShapeItem::SigNewCuttingShape, this, &MainWindow::SloNewCuttingShape);
}
void MainWindow::FunDrawShapeCommonStepForCutting(int type, bool direct, int x, int y, int width, int high, int point, int angel)
{
    //Init new shape item
    ClaShapeItem *tmpItem = new ClaShapeItem;
    QPainterPath startPath = tmpItem->FunSetItemConfig(type,direct,0,0,width - x,high - y,point,angel);
    tmpItem->setPen(pen);
    tmpItem->setPath(startPath);
    tmpItem->moveBy(x, y);

    //Add item to allGraphicsItems and show it to scene
    myClaScene->addItem(tmpItem);
    allGraphicsItems.push_back(tmpItem);

    //Init a little point for resize
    ClaPointReszie *tmpPoint = new ClaPointReszie(tmpItem);
    QPainterPath startPathPoint = tmpPoint->FunInitPointResize(width - x,high - y);
    tmpPoint->setPath(startPathPoint);

    //ListWidget should choosed item when shape has choosed
    connect(tmpItem, &ClaShapeItem::SigSelectedItemList, this, &MainWindow::SloSelectedItemList);
    //Reset the new path for Shape when resize point moved
    connect(tmpPoint, &ClaPointReszie::SigPointMoved, tmpItem, &ClaShapeItem::SloPointMoved);
    //Set new width and high of shape item just should renew position of resize point
    connect(tmpItem, &ClaShapeItem::SigSetNewWidthHigh, tmpPoint, &ClaPointReszie::SloPointMoved);
    //Cutting new shape items
    connect(tmpItem, &ClaShapeItem::SigNewCuttingShape, this, &MainWindow::SloNewCuttingShape);
}
void MainWindow::FunDrawLineOnScene(void)
{
    FunDrawShapeCommonStep(TypeLine,0,0,0,100,100,0,0);

    //ListWidget have to update
    emit SigRenewItemList();
}
void MainWindow::FunDrawRectOnScene(void)
{
    FunDrawShapeCommonStep(TypeRect,0,0,0,100,100,0,0);

    //ListWidget have to update
    emit SigRenewItemList();
}
void MainWindow::FunDrawArcOnScene(void)
{
    bool ok;
    int i = QInputDialog::getInt(this, tr("beginPoint"),
                                 tr("Input beginPoint:"), 0, -360, 360, 1, &ok);
    int j = QInputDialog::getInt(this, tr("arcAngle"),
                                 tr("Input arcAngle:"), 90, -360, 360, 1, &ok);

    FunDrawShapeCommonStep(TypeArc,0,0,0,100,100,i,j);

    //ListWidget have to update
    emit SigRenewItemList();
}
void MainWindow::FunDrawArcHorizOnScene(void)
{
    FunDrawShapeCommonStep(TypeArc,0,0,0,100,100,-90,180);

    //ListWidget have to update
    emit SigRenewItemList();
}
void MainWindow::FunDrawArcVertiOnScene(void)
{
    FunDrawShapeCommonStep(TypeArc,0,0,0,100,100,0,180);

    //ListWidget have to update
    emit SigRenewItemList();
}
void MainWindow::FunDrawCircleOnScene(void)
{
    FunDrawShapeCommonStep(TypeCircle,0,0,0,100,100,0,0);

    //ListWidget have to update
    emit SigRenewItemList();
}
#pragma endregion}

#pragma region ShapeOperate{
void MainWindow::FunOpenFile(void)
{
    QString strFile = QFileDialog::getOpenFileName( this,
                                                    tr("打开文件"),
                                                    tr("c:\\"),
                                                    tr("Text Files (*.txt);; All Files (*.*);; DXF Files (*.dxf)"));
    if(strFile.right(3) == "dxf")
    {
        ClaDxfAdapter  f;

        DL_Dxf dxf;
        if (!dxf.in(strFile.toStdString(), &f))
            ui->statusBar->showMessage("Open File Failed!",3000);

        for(int i = 0; i < f.shapeConfVec.size(); i++)
        {
            if(f.shapeConfVec.at(i).typeConf == TypeLine)
            {
                FunDrawShapeCommonStepForCutting(f.shapeConfVec.at(i).typeConf,
                                       f.shapeConfVec.at(i).directConf,
                                       f.shapeConfVec.at(i).xConf,
                                       f.shapeConfVec.at(i).yConf,
                                       f.shapeConfVec.at(i).widthConf,
                                       f.shapeConfVec.at(i).highConf,
                                       f.shapeConfVec.at(i).pointConf - 90,
                                       f.shapeConfVec.at(i).angleConf);
            }
            else
            {
                FunDrawShapeCommonStep(f.shapeConfVec.at(i).typeConf,
                                       f.shapeConfVec.at(i).directConf,
                                       f.shapeConfVec.at(i).xConf,
                                       f.shapeConfVec.at(i).yConf,
                                       f.shapeConfVec.at(i).widthConf,
                                       f.shapeConfVec.at(i).highConf,
                                       f.shapeConfVec.at(i).pointConf - 90,
                                       f.shapeConfVec.at(i).angleConf);
            }
        }
    }
    else
    {
        QFile file(strFile);
        if(!file.open(QIODevice::ReadOnly))
            ui->statusBar->showMessage("Open File Failed!",3000);
        else
            ui->statusBar->showMessage("Open File Successed!", 3000);

        QList<QString> allBuf;
        while(file.atEnd() == false)
        {
            QString buf = file.readLine();
            allBuf.push_back(buf);
        }

        QList<struShapeConf> allAnalyConf;
        ClaGcodeAnalyse *myClaGcodeAnalyse = new ClaGcodeAnalyse;
        myClaGcodeAnalyse->FunGcodeAnalyse(allBuf, allAnalyConf);

        file.close();

        //Draw path;
        for(int i = 0; i < allAnalyConf.size(); i++)
        {
            if(allAnalyConf.at(i).typeConf == TypeLine)
            {
                FunDrawShapeCommonStepForCutting(allAnalyConf.at(i).typeConf,
                                       allAnalyConf.at(i).directConf,
                                       allAnalyConf.at(i).xConf,
                                       allAnalyConf.at(i).yConf,
                                       allAnalyConf.at(i).widthConf,
                                       allAnalyConf.at(i).highConf,
                                       allAnalyConf.at(i).pointConf - 90,
                                       allAnalyConf.at(i).angleConf);
            }
            else
            {
                FunDrawShapeCommonStep(allAnalyConf.at(i).typeConf,
                                       allAnalyConf.at(i).directConf,
                                       allAnalyConf.at(i).xConf,
                                       allAnalyConf.at(i).yConf,
                                       allAnalyConf.at(i).widthConf,
                                       allAnalyConf.at(i).highConf,
                                       allAnalyConf.at(i).pointConf - 90,
                                       allAnalyConf.at(i).angleConf);
            }
        }
    }
    //ListWidget have to update
    emit SigRenewItemList();
}
void MainWindow::FunSaveFile(void)
{
    qDebug() << "save file";
}

void MainWindow::FunCopyShape(void)
{
    //Get all my selected items
    QList<QGraphicsItem *> itemList = myClaScene->selectedItems();

    ClaShapeOperate::FunCopyShapeToClipboard(itemList, allGraphicsItems, allCopyItems);
}
void MainWindow::FunPasteShape(void)
{
    //Traverse clipboard
    for(int i = 0; i < allCopyItems.size(); i++)
    {
        FunDrawShapeCommonStep( allCopyItems[i]->shapeType,allCopyItems[i]->makeDirect,allCopyItems[i]->posX,allCopyItems[i]->posY,
                                allCopyItems[i]->rectWidth,allCopyItems[i]->rectHigh,allCopyItems[i]->beginPoint,allCopyItems[i]->arcAngle);
    }

    //ListWidget have to update
    emit SigRenewItemList();
}

void MainWindow::FunSetColorOfPen(void)
{
    QColor color;
    QColorDialog dlg(this);
    dlg.setCurrentColor(color);
    if (dlg.exec() == QDialog::Accepted)
    {
        color = dlg.currentColor();
    }
    pen.setColor(color);
}

void MainWindow::FunDeleteOneShape(void)
{
    //Get all my selected items
    QList<QGraphicsItem *> itemList = myClaScene->selectedItems();

    ClaShapeOperate::FunDeleteShapeBothVectorAndScene(itemList, allGraphicsItems, myClaScene);

    //ListWidget have to update
    emit SigRenewItemList();
}
void MainWindow::FunClearAllShapes(void)
{
    ClaShapeOperate::FunClearShapeBothVectorAndScene(allGraphicsItems, myClaScene);

    //ListWidget have to update
    emit SigRenewItemList();
}

void MainWindow::FunSetClockwise(void)
{
    //FunSetClockCommonStep(0);
    QList<QGraphicsItem *> itemList = myClaScene->selectedItems();
    ClaShapeOperate::FunSetClockCommonStep(itemList, EnumClockwise);
}
void MainWindow::FunSetAnticlockwise(void)
{
    //FunSetClockCommonStep(1);
    QList<QGraphicsItem *> itemList = myClaScene->selectedItems();
    ClaShapeOperate::FunSetClockCommonStep(itemList, EnumAntiClockwise);
}

void MainWindow::FunSetShapeRank(void)
{
    QList<ClaShapeItem *> newAllGraphicsItems = ClaShapeOperate::FunSetShapeRankCommonStep(ui->listWidget, allGraphicsItems);
    allGraphicsItems.clear();
    allGraphicsItems = newAllGraphicsItems;

    //ListWidget have to update
    emit SigRenewItemList();
}
void MainWindow::FunAllShowMess(void)
{
    /*
    for(int i = 0; i < allGraphicsItems.size(); i++)
    {
        if(TypeCircle == allGraphicsItems.at(i)->shapeType || TypeArc == allGraphicsItems.at(i)->shapeType)
        {
//            qDebug() << i
//                     << ":" << allGraphicsItems.at(i)->scenePos().x() << "," << allGraphicsItems.at(i)->scenePos().y()
//                     << " " << allGraphicsItems.at(i)->rectWidth << "," << allGraphicsItems.at(i)->rectHigh
//                     << " " << allGraphicsItems.at(i)->beginPoint << "," << allGraphicsItems.at(i)->arcAngle;

            double realX, realY;
            FunGetPositionByAngle(allGraphicsItems.at(i)->beginPoint,
                                  allGraphicsItems.at(i)->scenePos().x() + allGraphicsItems.at(i)->rectWidth / 2,
                                  allGraphicsItems.at(i)->scenePos().y() + allGraphicsItems.at(i)->rectHigh / 2,
                                  allGraphicsItems.at(i)->rectWidth / 2,
                                  realX,
                                  realY);
            qDebug() << "beginPoint Posi:" << (int)realX << "/" << (int)realY;

            FunGetPositionByAngle(allGraphicsItems.at(i)->beginPoint + allGraphicsItems.at(i)->arcAngle,
                                  allGraphicsItems.at(i)->scenePos().x() + allGraphicsItems.at(i)->rectWidth / 2,
                                  allGraphicsItems.at(i)->scenePos().y() + allGraphicsItems.at(i)->rectHigh / 2,
                                  allGraphicsItems.at(i)->rectWidth / 2,
                                  realX,
                                  realY);
            qDebug() << "endPoint Posi:" << (int)realX << "/" << (int)realY;

            FunGetPositionByAngle((allGraphicsItems.at(i)->beginPoint + allGraphicsItems.at(i)->arcAngle) / 2,
                                  allGraphicsItems.at(i)->scenePos().x() + allGraphicsItems.at(i)->rectWidth / 2,
                                  allGraphicsItems.at(i)->scenePos().y() + allGraphicsItems.at(i)->rectHigh / 2,
                                  allGraphicsItems.at(i)->rectWidth / 2,
                                  realX,
                                  realY);
            qDebug() << "midPoint Posi:" << (int)realX << "/" << (int)realY;
        }
    }
    */

    for(int i = 0; i < allGraphicsItems.size(); i++)
    {
        qDebug() << i << " : " << allGraphicsItems.at(i)->scenePos().x() << "," << allGraphicsItems.at(i)->scenePos().y();
        qDebug() << i << " : " << allGraphicsItems.at(i)->posX << "," << allGraphicsItems.at(i)->posY << endl;
    }
}

void MainWindow::FunShowWaveLine(void)
{
    DialogShowWaveLine *myDialogShowWaveLine = new DialogShowWaveLine(nullptr, service);
    myDialogShowWaveLine->show();
}
#pragma endregion}

#pragma region Slots{
void MainWindow::SloRenewItemList(void)
{
    //Update items of listWidget by new allGraphicsItems
    ui->listWidget->clear();
    for(int i = 0; i < allGraphicsItems.size(); i++)
    {
        //Order number + Item address
        ui->listWidget->addItem(QString::number(i+1) + ": " + QString::number(allGraphicsItems[i]->addrName,16));
    }
}
void MainWindow::SloSelectedItemList(int addrName)
{
    int listCount = ui->listWidget->count();
    for(int i = 0; i < listCount; i++)
    {
        //ListWidget should choosed item when shape has choosed
        if( QString::number(addrName,16) == ui->listWidget->item(i)->text().mid(3,ui->listWidget->item(i)->text().size()))
        {
            ui->listWidget->item(i)->setSelected(true);
        }
    }

}
void MainWindow::SloNewCuttingShape(QList<struShapeConf> &newShapeVec)
{
    for(int i = 0; i < newShapeVec.size(); i++)
    {
        FunDrawShapeCommonStepForCutting(newShapeVec.at(i).typeConf,
                               newShapeVec.at(i).directConf,
                               newShapeVec.at(i).xConf,
                               newShapeVec.at(i).yConf,
                               newShapeVec.at(i).widthConf,
                               newShapeVec.at(i).highConf,
                               newShapeVec.at(i).pointConf,
                               newShapeVec.at(i).angleConf);
    }
    //ListWidget have to update
    emit SigRenewItemList();
}
#pragma endregion}


#pragma region PLC{
void MainWindow::FunGetRealPositionOfPreDraw(ClaShapeItem *nowPreDraw, int &realBeginX, int &realBeginY, int &realEndX, int &realEndY)
{
    qreal prex,prey,prew,preh;
    nowPreDraw->path().boundingRect().getRect(&prex, &prey, &prew, &preh);

    realBeginX = nowPreDraw->scenePos().x() + prex;
    realBeginY = nowPreDraw->scenePos().y() + prey;
    realEndX = realBeginX + prew;
    realEndY = realBeginY + preh;
}

void MainWindow::FunGetRealPositionOfPreLine(ClaShapeItem *nowPreLine, int &realBeginX, int &realBeginY, int &realEndX, int &realEndY)
{
    qreal prex,prey,prew,preh;
    nowPreLine->path().boundingRect().getRect(&prex, &prey, &prew, &preh);

    realBeginX = nowPreLine->scenePos().x();
    realBeginY = nowPreLine->scenePos().y();

    //EndX
    if(0 == prex)
        realEndX = realBeginX + prew;
    else
        realEndX = realBeginX + prex;

    //EndY
    if(0 == prey)
        realEndY = realBeginY + preh;
    else
        realEndY = realBeginY + prey;
}

void MainWindow::FunBtnClickedConnectPlc(void)
{
    if(ui->btnConnetPlc->text() == "Connect PLC")
    {
        char *plcIp;
        if(ui->lnedPlcIp->text().isEmpty())
        {
            plcIp = "192.168.6.6";
        }
        else
        {
            plcIp = ui->lnedPlcIp->text().toLatin1().data();
        }

        int connectResu = service->SetCommunication(plcIp);
        if(connectResu != 0)
        {
            ui->statusBar->showMessage("Connect Failed!",3000);
            return;
        }

        ui->statusBar->showMessage("Connect Success!",3000);
        ui->btnConnetPlc->setText("DisConnect");

        ui->btnMakeSpinCan->setEnabled(true);
    }
    else
    {
        service->CloseXNetWindows();
        ui->btnConnetPlc->setText("Connect PLC");
        ui->statusBar->showMessage("Disconnected!",3000);

        ui->btnMakeSpinCan->setText("Motor Enable");
        ui->btnMakeSpinCan->setEnabled(false);
        ui->btnWorkBeginStop->setText("Work Begin");
        ui->btnWorkBeginStop->setEnabled(false);
        ui->btnWorkHaltContinue->setText("Work Halt");
        ui->btnWorkHaltContinue->setEnabled(false);
    }
}
void MainWindow::FunBtnClickedMotorEnable(void)
{
    if(ui->btnMakeSpinCan->text() == "Motor Enable")
    {
        //轴组使能
        service->WriteRegs(XNet_M,0,1,&numOn);

        ui->btnMakeSpinCan->setText("Motor Disenable");

        ui->btnWorkBeginStop->setEnabled(true);
        ui->btnWorkHaltContinue->setEnabled(true);
    }
    else
    {
        //轴组停止使能
        service->WriteRegs(XNet_M,0,1,&numOff);

        ui->btnMakeSpinCan->setText("Motor Enable");

        ui->btnWorkBeginStop->setText("Work Begin");
        ui->btnWorkBeginStop->setEnabled(false);
        ui->btnWorkHaltContinue->setText("Work Halt");
        ui->btnWorkHaltContinue->setEnabled(false);
    }
}

void MainWindow::FunBtnClickedWorkBeginStop(void)
{
    if(ui->btnWorkBeginStop->text() == "Work Begin")
    {
        ui->btnWorkBeginStop->setText("Work Stop");

        FunGetAllShapeMovePathes();
    }
    else
    {
        ui->btnWorkBeginStop->setText("Work Begin");

        //给M100上升沿 回原点结束一切
        service->WriteRegs(XNet_M,100,1,&numOn);
        ClaTimer::TimeSleep(50);
        service->WriteRegs(XNet_M,100,1,&numOff);
    }
}
void MainWindow::FunBtnClickedWorkHaltContinue(void)
{
    if(ui->btnWorkHaltContinue->text() == "Work Halt")
    {
        ui->btnWorkHaltContinue->setText("Work Continue");

        //给M20上升沿 驱动三轴暂停
        service->WriteRegs(XNet_M,30,1,&numOn);
        ClaTimer::TimeSleep(50);
        service->WriteRegs(XNet_M,30,1,&numOff);
    }
    else
    {
        ui->btnWorkHaltContinue->setText("Work Halt");

        //给M30上升沿 驱动三轴继续
        service->WriteRegs(XNet_M,12,1,&numOn);
        ClaTimer::TimeSleep(50);
        service->WriteRegs(XNet_M,12,1,&numOff);
    }
}

void MainWindow::FunGetPositionByAngle(int angle, int centerX, int centerY, int r, double &realX, double &realY)
{
    int realAngle = angle + 90;

    realX = centerX + r * cos(realAngle * 3.14 / 180);
    realY = centerY + r * sin((realAngle - 180) * 3.14 / 180);
}

void MainWindow::FunGetAllShapeMovePathes(void)
{
    short lineCount = 0;

    for(int i = 0; i < allGraphicsItems.size(); i++)
    {
        int realBeginX, realBeginY, realEndX, realEndY;

        if(allGraphicsItems.at(i)->shapeType == TypeLine)
        {
            FunGetRealPositionOfPreLine(allGraphicsItems.at(i), realBeginX, realBeginY, realEndX, realEndY);
            //PTP
            struPlcLineConf ptpStru;
            ptpStru.lineNum = ++lineCount;
            ptpStru.moveType = 0;
            ptpStru.targetX = (double)(realBeginX * 1000);
            ptpStru.targetY = (double)(realBeginY * 1000);
            ptpStru.targetZ = (double)(0);
            ptpStru.supportX = 0;
            ptpStru.supportY = 0;
            ptpStru.supportZ = 0;
            plcLineConfVec.push_back(ptpStru);

            //Line
            struPlcLineConf lineStru;
            lineStru.lineNum = ++lineCount;
            lineStru.moveType = 1;
            lineStru.targetX = (double)(realEndX * 1000);
            lineStru.targetY = (double)(realEndY * 1000);
            lineStru.targetZ = (double)(0);
            lineStru.supportX = 0;
            lineStru.supportY = 0;
            lineStru.supportZ = 0;
            plcLineConfVec.push_back(lineStru);
        }
        else if(allGraphicsItems.at(i)->shapeType == TypeRect)
        {
            FunGetRealPositionOfPreDraw(allGraphicsItems.at(i), realBeginX, realBeginY, realEndX, realEndY);
            //PTP
            struPlcLineConf ptpStru;
            ptpStru.lineNum = ++lineCount;
            ptpStru.moveType = 0;
            ptpStru.targetX = (double)(realBeginX * 1000);
            ptpStru.targetY = (double)(realBeginY * 1000);
            ptpStru.targetZ = (double)(0);
            ptpStru.supportX = 0;
            ptpStru.supportY = 0;
            ptpStru.supportZ = 0;
            plcLineConfVec.push_back(ptpStru);

            if(allGraphicsItems.at(i)->makeDirect == EnumClockwise)//顺时针
            {
                //Line1-4
                struPlcLineConf lineStru1;
                lineStru1.lineNum = ++lineCount;
                lineStru1.moveType = 1;
                lineStru1.targetX = (double)(realEndX * 1000);
                lineStru1.targetY = (double)(realBeginY * 1000);
                lineStru1.targetZ = (double)(0);
                lineStru1.supportX = 0;
                lineStru1.supportY = 0;
                lineStru1.supportZ = 0;

                struPlcLineConf lineStru2;
                lineStru2.lineNum = ++lineCount;
                lineStru2.moveType = 1;
                lineStru2.targetX = (double)(realEndX * 1000);
                lineStru2.targetY = (double)(realEndY * 1000);
                lineStru2.targetZ = (double)(0);
                lineStru2.supportX = 0;
                lineStru2.supportY = 0;
                lineStru2.supportZ = 0;

                struPlcLineConf lineStru3;
                lineStru3.lineNum = ++lineCount;
                lineStru3.moveType = 1;
                lineStru3.targetX = (double)(realBeginX * 1000);
                lineStru3.targetY = (double)(realEndY * 1000);
                lineStru3.targetZ = (double)(0);
                lineStru3.supportX = 0;
                lineStru3.supportY = 0;
                lineStru3.supportZ = 0;

                struPlcLineConf lineStru4;
                lineStru4.lineNum = ++lineCount;
                lineStru4.moveType = 1;
                lineStru4.targetX = (double)(realBeginX * 1000);
                lineStru4.targetY = (double)(realBeginY * 1000);
                lineStru4.targetZ = (double)(0);
                lineStru4.supportX = 0;
                lineStru4.supportY = 0;
                lineStru4.supportZ = 0;

                plcLineConfVec.push_back(lineStru1);
                plcLineConfVec.push_back(lineStru2);
                plcLineConfVec.push_back(lineStru3);
                plcLineConfVec.push_back(lineStru4);
            }
            else//逆时针
            {
                //Line1-4
                struPlcLineConf lineStru1;
                lineStru1.lineNum = ++lineCount;
                lineStru1.moveType = 1;
                lineStru1.targetX = (double)(realBeginX * 1000);
                lineStru1.targetY = (double)(realEndY * 1000);
                lineStru1.targetZ = (double)(0);
                lineStru1.supportX = 0;
                lineStru1.supportY = 0;
                lineStru1.supportZ = 0;

                struPlcLineConf lineStru2;
                lineStru2.lineNum = ++lineCount;
                lineStru2.moveType = 1;
                lineStru2.targetX = (double)(realEndX * 1000);
                lineStru2.targetY = (double)(realEndY * 1000);
                lineStru2.targetZ = (double)(0);
                lineStru2.supportX = 0;
                lineStru2.supportY = 0;
                lineStru2.supportZ = 0;

                struPlcLineConf lineStru3;
                lineStru3.lineNum = ++lineCount;
                lineStru3.moveType = 1;
                lineStru3.targetX = (double)(realEndX * 1000);
                lineStru3.targetY = (double)(realBeginY * 1000);
                lineStru3.targetZ = (double)(0);
                lineStru3.supportX = 0;
                lineStru3.supportY = 0;
                lineStru3.supportZ = 0;

                struPlcLineConf lineStru4;
                lineStru4.lineNum = ++lineCount;
                lineStru4.moveType = 1;
                lineStru4.targetX = (double)(realBeginX * 1000);
                lineStru4.targetY = (double)(realBeginY * 1000);
                lineStru4.targetZ = (double)(0);
                lineStru4.supportX = 0;
                lineStru4.supportY = 0;
                lineStru4.supportZ = 0;

                plcLineConfVec.push_back(lineStru1);
                plcLineConfVec.push_back(lineStru2);
                plcLineConfVec.push_back(lineStru3);
                plcLineConfVec.push_back(lineStru4);
            }
        }
        else if(allGraphicsItems.at(i)->shapeType == TypeArc)
        {
            //同样的流程吧可能

        }
        else if(allGraphicsItems.at(i)->shapeType == TypeCircle)
        {
            //同样的流程吧可能
        }
    }

    //End
    struPlcLineConf endStru;
    endStru.lineNum = ++lineCount;
    endStru.moveType = 200;
    endStru.targetX = 0;
    endStru.targetY = 0;
    endStru.supportX = 0;
    endStru.supportY = 0;
    plcLineConfVec.push_back(endStru);

    myClaDrawByPlc->FunGetPlcLineConf(service, plcLineConfVec);

    plcLineConfVec.clear();
    short tmpnum = 0;
    service->WriteRegs(XNet_HD, HD_LINE_BEGIN, 1, &tmpnum);
}
#pragma endregion}
