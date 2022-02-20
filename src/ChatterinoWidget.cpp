#include "ChatterinoWidget.h"

ChatterinoWidget::ChatterinoWidget(QWidget *parent)
    : QWidget{parent}
{
    this->installEventFilter(this);
}

bool ChatterinoWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent * mouseEvent = static_cast <QMouseEvent *> (event);

        if (mouseEvent -> button() == Qt::RightButton)
        {
            qDebug() << "riught";
            emit chatterinoRightMouseClick(mouseEvent->globalPos());
        }
    }
    return QWidget::eventFilter(obj, event);
}
