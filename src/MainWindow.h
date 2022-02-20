#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt Compatibility

#include <thread>


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

protected:
    void resizeEvent(QResizeEvent * event);

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
};
#endif // MAINWINDOW_H
