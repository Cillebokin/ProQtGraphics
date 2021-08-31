#ifndef CLADXFADAPTER_H
#define CLADXFADAPTER_H

#include <QObject>
#include "dl_creationadapter.h"
#include "dl_dxf.h"

#include "clashapeitem.h"

class ClaDxfAdapter : public DL_CreationAdapter
{
public:
    explicit ClaDxfAdapter();
    ~ClaDxfAdapter();

    QList<struShapeConf> shapeConfVec;

    virtual void addLine(const DL_LineData& d);
    virtual void addArc(const DL_ArcData& d);
    virtual void addCircle(const DL_CircleData& d);
};

#endif // CLADXFADAPTER_H
