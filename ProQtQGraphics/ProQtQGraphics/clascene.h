#ifndef CLASCENE_H
#define CLASCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include "clashapeitem.h"

class ClaScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit ClaScene(QObject *parent = 0);
    ~ClaScene();

protected:
    //virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)override;


signals:

public slots:
};

#endif // CLASCENE_H
