#ifndef CLACUTTINGSCENE_H
#define CLACUTTINGSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPathItem>

class ClaCuttingScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit ClaCuttingScene(QObject *parent = 0);
    ~ClaCuttingScene();

    int beginX;
    int beginY;
    int endX;
    int endY;

    QGraphicsPathItem *myQGraphicsPathItem;

protected:
    virtual void    mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)override;
    virtual void	mousePressEvent (QGraphicsSceneMouseEvent * mouseEvent );
    virtual void	mouseReleaseEvent (QGraphicsSceneMouseEvent * mouseEvent );

signals:

public slots:
};

#endif // CLACUTTINGSCENE_H
