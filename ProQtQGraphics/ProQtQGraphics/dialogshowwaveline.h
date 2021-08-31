#ifndef DIALOGSHOWWAVELINE_H
#define DIALOGSHOWWAVELINE_H

#include <QDialog>
#include <QGraphicsScene>

#include "x_net.h"

#include "threadgetxy.h"

namespace Ui {
class DialogShowWaveLine;
}

class DialogShowWaveLine : public QDialog
{
    Q_OBJECT

public:
    DialogShowWaveLine(QWidget *parent, X_Net *outService);
    ~DialogShowWaveLine();
    void DialogShowWaveLineDeconstruction();

private:
    Ui::DialogShowWaveLine *ui;

    QGraphicsScene *myQGraphicsScene;
    X_Net *service;

    ThreadGetXY *myThreadGetXY;

    QPen pen;

    void FunInitUi(void);
    void FunConnect(void);

    void FunBeginOrStop(void);
    void FunClearScene(void);
    void FunCloseDialog(void);

protected:

public slots:
    void SloGetedXAndY(double posiX, double posiY);

};

#endif // DIALOGSHOWWAVELINE_H
