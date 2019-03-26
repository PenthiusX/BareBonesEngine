#ifndef _TEST_H
#define _TEST_H

#include <QObject>

class _Test : public QObject
{
    Q_OBJECT
public:
    explicit _Test(QObject *parent = nullptr);

signals:

public slots:
};

#endif // _TEST_H