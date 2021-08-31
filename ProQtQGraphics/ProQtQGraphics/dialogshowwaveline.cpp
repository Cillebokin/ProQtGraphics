#include "dialogshowwaveline.h"
#include "ui_dialogshowwaveline.h"
#include <QDebug>

DialogShowWaveLine::DialogShowWaveLine(QWidget *parent, X_Net *outService) :
    QDialog(parent),
    ui(new Ui::DialogShowWaveLine)
{
    Q_UNUSED(outService);

    ui->setupUi(this);

    FunInitUi();
    FunConnect();
    //service = outService;
    service = new X_Net;
    service->SetCommunication("192.168.6.6");

    myThreadGetXY = new ThreadGetXY(service);
    connect(myThreadGetXY, SIGNAL(SigGetedXAndY(double,double)), this, SLOT(SloGetedXAndY(double,double)));

    pen.setColor(Qt::red);
    pen.setWidth(3);
}

DialogShowWaveLine::~DialogShowWaveLine()
{
    delete ui;
}

void DialogShowWaveLine::DialogShowWaveLineDeconstruction()
{
    myThreadGetXY->stopFlag = true;
    QDialog::close();
}
//===================================================================
void DialogShowWaveLine::FunInitUi(void)
{
    this->setFixedSize(1071,850);
    this->setWindowTitle("Show Wave Line");

    myQGraphicsScene = new QGraphicsScene;
    myQGraphicsScene->setBackgroundBrush(Qt::white);
    ui->graphicsView->setScene(myQGraphicsScene);

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

    myQGraphicsScene->setBackgroundBrush(pix);
}

void DialogShowWaveLine::FunConnect(void)
{
    connect(ui->btnBeginOrStop, &QPushButton::clicked, this, &DialogShowWaveLine::FunBeginOrStop);
    connect(ui->btnClear, &QPushButton::clicked, this, &DialogShowWaveLine::FunClearScene);

    connect(ui->btnClose, &QPushButton::clicked, this, &DialogShowWaveLine::DialogShowWaveLineDeconstruction);

}
//===================================================================
void DialogShowWaveLine::FunBeginOrStop(void)
{
    if(ui->btnBeginOrStop->text() == "Begin")
    {
        ui->btnBeginOrStop->setText("Stop");
        myThreadGetXY->start();
    }
    else
    {
        ui->btnBeginOrStop->setText("Begin");
        myThreadGetXY->stopFlag = true;
    }
}
void DialogShowWaveLine::FunClearScene(void)
{
    myQGraphicsScene->clear();
}
//===================================================================
void DialogShowWaveLine::SloGetedXAndY(double posiX, double posiY)
{
    //Show Mess
    QString str = QString::number(posiX) + "," + QString::number(posiY);
    ui->labPosiXY->setText(str);

    //Draw Line
    int realX = (int)(posiX / 1000);
    int realY = (int)(posiY / 1000);
    myQGraphicsScene->addLine(realX,realY,realX,realY,pen);
}

