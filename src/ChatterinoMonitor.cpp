#include "ChatterinoMonitor.h"

ChatterinoMonitor::ChatterinoMonitor(QString homePath, QObject *parent)
    : QObject{parent}
{
    db "start";
    _windowLayoutJsonPath.prepend(homePath);
    _windowLayoutTimer = new QTimer;
    connect(_windowLayoutTimer, &QTimer::timeout, this, [=]()
    {
        QFileInfo f(_windowLayoutJsonPath);
        QDateTime dt = f.lastModified();
//        db _windowLayoutLastModified;
//        db dt;
//        db _windowLayoutJsonPath;
        if (_windowLayoutLastModified != dt)
        {
            _windowLayoutLastModified = dt;
            windowLayoutUpdated();
        }
    });
    _windowLayoutTimer->start(200);
}

void ChatterinoMonitor::windowLayoutUpdated()
{
    db "updated";
    QFile f(_windowLayoutJsonPath);
    if(!f.open(QFile::ReadOnly))
        qWarning() << "Unable to open file: " << _windowLayoutJsonPath << f.errorString();

    QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    QJsonArray tabs = doc.object()["windows"].toArray()[0].toObject()["tabs"].toArray();
    foreach(QJsonValue obj, tabs)
    {
        if(obj.toObject()["selected"].toBool(false))
        {
            db obj.toObject()["splits2"].toObject()["data"].toObject()["name"].toString();
            return;
        }
    }
}
