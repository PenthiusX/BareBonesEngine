#ifndef _PROCESSING_H
#define _PROCESSING_H

#include <QObject>

class _Processing : public QObject
{
    Q_OBJECT
public:
    explicit _Processing(QObject *parent = nullptr);
    void init();

signals:

public slots:

protected:
};

#endif // _PROCESSING_H
