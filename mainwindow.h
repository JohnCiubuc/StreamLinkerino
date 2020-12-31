#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt Compatibility

#include <thread>


#include <QMainWindow>
#include <QTimer>
#include <QResizeEvent>

#include <QProcess>
#include <QWindow>
#include <QDebug>
#include <QFileSystemWatcher>



#include <QProcessEnvironment>


#include "windowsmatchingpid.h"

//    streamlinkArgs<< "--twitch-low-latency"<< "--twitch-disable-ads"<<"--player"<<"mpv --wid="+QString::number(mpv_window->winId()) ;

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
    void chatChannelMonitor();
    void readStreamLink();

protected:
    void resizeEvent(QResizeEvent * event);

private:
    void resizeEmbeds();
    QStringList createStreamLinkArgs(QString channel);

    Ui::MainWindow *ui;
    WindowsMatchingPid _WMP;
    QWidget * _chatContainer;
    QWidget * _mpvContainer;
    bool _bChatterinoEmbedded = false;
    QProcess * _pStreamlinkProcess;
    QProcess * _pChatterinoProcess;
    QTimer * _tChatChannelMonitor;
    QByteArray _cChatChannel;
    QByteArray _chatterinoUUID;
    unsigned long _mpvContainerWID;
};
#endif // MAINWINDOW_H
