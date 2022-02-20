#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt Compatibility

#include <thread>

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

#include <QAbstractEventDispatcher>
#include <QDir>
#include <QMainWindow>
#include <QTimer>
#include <QResizeEvent>
#include <QProcess>
#include <QWindow>
#include <QDebug>
#include <QMenu>
#include <QFileSystemWatcher>
#include "WindowsMatchingPID.h"
#include "Submodules.h"
#include "ChatterinoMonitor.h"
#include "ChatterinoWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void setupChatterinoEmbed();
    void chatterinoMonitor();
    void readStreamLink();
    void initialize();
    void refreshStream();
    void changeChannel(QByteArray channel = "");

protected:
    void resizeEvent(QResizeEvent * event);
    bool eventFilter(QObject *obj, QEvent *event);
    bool x11EventFilter(XEvent * event);
private:
    void resizeEmbeds();
    QString generateStatusHTML(bool bPrerollAds = false);

    Ui::MainWindow *ui;
    ChatterinoMonitor * _CM;

    QMenu * forceLoadMenu;

    WindowsMatchingPid _WMP;
    Submodules::SubmodulesDialog * _Submodules;
    QWidget * _chatContainer;
    QWidget * _mpvContainer;
    bool _bChatterinoEmbedded = false;
    QList<QProcess *> _pStreamlinkProcess;
    QProcess * _pChatterinoProcess;
    QTimer * _tChatChannelMonitor;
    QByteArray _cChatChannel;
    QByteArray _chatterinoUUID;
    unsigned long _mpvContainerWID;
    bool _bStreamLinkProcessSelector = true;
    bool _bStreamlinkAllowSwitching = false;
    bool _bLoadFinished = false;
    QByteArray _pendingChannelLoad;

    ChatterinoWidget * CW;
};
#endif // MAINWINDOW_H
