#ifndef _TABBAR_H
#define _TABBAR_H

#include <QApplication>
#include <QStyleOptionTab>
#include <QStylePainter>
#include <QTabBar>
#include <QTabWidget>


class _TabBar: public QTabBar
{
public:
    QSize tabSizeHint(int index) const;
protected:
    void paintEvent(QPaintEvent * /*event*/);
};
#endif // _TABBAR_H
