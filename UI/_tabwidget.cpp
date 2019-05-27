#include "_tabwidget.h"
#include <UI/_tabbar.h>

_TabWidget::_TabWidget(QWidget *parent) : QTabWidget(parent){
    setTabBar(new _TabBar);
    setTabPosition(QTabWidget::West);
}
