#include "ChatterinoWidget.h"

ChatterinoWidget::ChatterinoWidget(QWidget *parent)
    : QLabel{parent}
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
            qDebug() << "riught through this";
            emit chatterinoRightMouseClick(mouseEvent->globalPos());
            emit gotEvent(obj, event);
        }
    }
    return QWidget::eventFilter(obj, event);
}
