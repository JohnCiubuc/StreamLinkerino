#ifndef DEBUGOVERLAY_H
#define DEBUGOVERLAY_H

#include <QDialog>
#include <QObject>
#include <QWidget>
#include <QLabel>
//#include <QEvent>
#include <QDebug>
#include <QMouseEvent>

class DebugOverlay : public QDialog
{
    Q_OBJECT
public:
    explicit DebugOverlay(QWidget *parent = nullptr);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // DEBUGOVERLAY_H
