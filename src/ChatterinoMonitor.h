#ifndef CHATTERINOMONITOR_H
#define CHATTERINOMONITOR_H

#include <QObject>
#include <QJsonDocument>
#include <QTimer>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonArray>

#define db qDebug() << this <<
class ChatterinoMonitor : public QObject
{
    Q_OBJECT
public:
    explicit ChatterinoMonitor(QString homePath, QObject *parent = nullptr);

signals:
    void changeChannel(QByteArray);
private slots:
    void windowLayoutUpdated();
private:


    QTimer * _windowLayoutTimer;


    QDateTime _windowLayoutLastModified;
    QString _windowLayoutJsonPath = "/.local/share/chatterino/Settings/window-layout.json";
    QByteArray _activeChannel;
};

#endif // CHATTERINOMONITOR_H
