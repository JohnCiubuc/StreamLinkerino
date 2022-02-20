#ifndef CHATTERINOMONITOR_H
#define CHATTERINOMONITOR_H

#include <QObject>
#include <QJsonDocument>
#include <QFileSystemWatcher>

class ChatterinoMonitor : public QObject
{
    Q_OBJECT
public:
    explicit ChatterinoMonitor(QObject *parent = nullptr);

signals:

};

#endif // CHATTERINOMONITOR_H
