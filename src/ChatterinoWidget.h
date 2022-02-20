#ifndef CHATTERINOWIDGET_H
#define CHATTERINOWIDGET_H

#include <QObject>
#include <QWidget>
//#include <QEvent>
#include <QDebug>
#include <QMouseEvent>
class ChatterinoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChatterinoWidget(QWidget *parent = nullptr);

signals:
    void chatterinoRightMouseClick(QPoint);
protected:
    bool eventFilter(QObject *obj, QEvent *event);;

};

#endif // CHATTERINOWIDGET_H
