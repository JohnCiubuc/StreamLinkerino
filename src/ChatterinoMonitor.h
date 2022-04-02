#ifndef CHATTERINOMONITOR_H
#define CHATTERINOMONITOR_H

#include <X11/Xlib.h>

#undef Bool
#undef CursorShape
#undef Expose
#undef KeyPress
#undef KeyRelease
#undef FocusIn
#undef FocusOut
#undef FontChange
#undef None
#undef Status
#undef Unsorted

#include <QObject>
#include <QJsonDocument>
#include <QTimer>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonArray>
#include <QProcess>
//#include <X11/Xlib.h>

#define db qDebug() << this <<
class ChatterinoMonitor : public QObject
{
    Q_OBJECT
signals:
    void changeChannel(QByteArray);

    void reloadChatterino();
public:
    explicit ChatterinoMonitor(QString homePath, QObject *parent = nullptr);
    void updatePID(int pid)
    {
        _pid = pid;
        _pidString = QString::number(pid);
    }
    QString getTempScriptFile()
    {
        return _tempFileName;
    }
public slots:
    void checkWindows();
    void closeWindow(Window w);
private slots:
    void windowLayoutUpdated();
    QString getWindowName(Window win);
    int client_msg(Window win);

private:


    QTimer * _windowLayoutTimer;


    QDateTime _windowLayoutLastModified;
    QString _windowLayoutJsonPath = "/.local/share/chatterino/Settings/window-layout.json";
    QByteArray _activeChannel;

// Honestly everything here is linux, but just for temporary specification
#ifdef __linux__
    int _windowListSize;
    int _pid = -1;
    QString _pidString;
    Display * _display;
    QString _tempFileName = "/tmp/streamlinkerino-read-windows";
#endif
};

#endif // CHATTERINOMONITOR_H
