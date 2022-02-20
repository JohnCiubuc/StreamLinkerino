#ifndef CHATTERINOWIDGET_H
#define CHATTERINOWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
//#include <QEvent>
#include <QDebug>
#include <QMouseEvent>
class ChatterinoWidget : public QLabel
{
    Q_OBJECT
public:
    explicit ChatterinoWidget(QWidget *parent = nullptr);

signals:
    void chatterinoRightMouseClick(QPoint);
    void gotEvent(QObject*, QEvent *);
protected:
    bool eventFilter(QObject *obj, QEvent *event);;

};

#endif // CHATTERINOWIDGET_H
