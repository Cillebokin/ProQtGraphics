#ifndef CLATIMER_H
#define CLATIMER_H

#include <QObject>

class ClaTimer : public QObject
{
    Q_OBJECT
public:
    explicit ClaTimer(QObject *parent = 0);
    ~ClaTimer();

    static void TimeSleep(unsigned int mesc);

signals:

public slots:
};

#endif // CLATIMER_H
