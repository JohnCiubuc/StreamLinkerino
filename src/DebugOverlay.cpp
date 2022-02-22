#include "DebugOverlay.h"

DebugOverlay::DebugOverlay(QWidget *parent)
    : QDialog{parent}
{

    this->installEventFilter(this);
}

bool DebugOverlay::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent * mouseEvent = static_cast <QMouseEvent *> (event);

        if (mouseEvent -> button() == Qt::RightButton)
        {
            qDebug() << "DO Through this";
//            emit chatterinoRightMouseClick(mouseEvent->globalPos());
//            emit gotEvent(obj, event);
        }
    }
    return QWidget::eventFilter(obj, event);
}
