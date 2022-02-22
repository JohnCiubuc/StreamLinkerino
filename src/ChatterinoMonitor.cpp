#include "ChatterinoMonitor.h"

ChatterinoMonitor::ChatterinoMonitor(QString homePath, QObject *parent)
    : QObject{parent}
{
    _display = XOpenDisplay(0);
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

void ChatterinoMonitor::checkWindows()
{
    if (_pid == -1) return;
    QProcess p;
    p.setProgram("/bin/bash");
    p.setArguments(QStringList() << "/tmp/streamlinkerino-read-windows" << _pidString);
    p.start();
    if(p.waitForFinished(1000))
    {
        QList<QByteArray> split = p.readAll().split('\n');
        // Last will always be an empty string as per current script
        split.removeLast();
        if (_windowListSize != split.size())
        {
            if (_windowListSize < split.size())
            {
                bool b;
                QString name = getWindowName(split.last().toUInt(&b,16));
                if (name.compare("Rename Tab") == 0)
                {
                    emit reloadChatterino();
                }
            }
            _windowListSize = split.size();
        }
    }
}

void ChatterinoMonitor::closeWindow(Window w)
{
    client_msg(w);
//    client_msg(_display, w, "_NET_CLOSE_WINDOW",
//               0, 0, 0, 0, 0);
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
            QByteArray channel = obj.toObject()["splits2"].toObject()["data"].toObject()["name"].toString().toLocal8Bit();
            if (_activeChannel.compare(channel) != 0)
            {
                _activeChannel = channel;
                emit changeChannel(channel);
            }
            return;
        }
    }
}

QString ChatterinoMonitor::getWindowName(Window win)
{
    int form;
    unsigned long remain, len;
    unsigned char *list;

    Atom prop = XInternAtom(_display,"WM_NAME",False), type;
    if (XGetWindowProperty(_display,win,prop,0,1024,False,AnyPropertyType,
                           &type,&form,&len,&remain,&list) != Success)   // XA_STRING
        return NULL;

    return QString(reinterpret_cast< char* >(list));
}

int ChatterinoMonitor::client_msg(Window win)
{
    XEvent event;
    long mask = SubstructureRedirectMask | SubstructureNotifyMask;

    event.xclient.type = ClientMessage;
    event.xclient.serial = 0;
    event.xclient.send_event = True;
    event.xclient.message_type = XInternAtom(_display, "_NET_CLOSE_WINDOW", False);
    event.xclient.window = win;
    event.xclient.format = 32;
//    event.xclient.data.l[0] = data0;
//    event.xclient.data.l[1] = data1;
//    event.xclient.data.l[2] = data2;
//    event.xclient.data.l[3] = data3;
//    event.xclient.data.l[4] = data4;

    if (XSendEvent(_display, DefaultRootWindow(_display), False, mask, &event))
    {
        return EXIT_SUCCESS;
    }
    else
    {
        qDebug() << "failed to send event" << "_NET_CLOSE_WINDOW";
//        fprintf(stderr, "Cannot send %s event.\n", msg);
        return EXIT_FAILURE;
    }
}
