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
#include "windowsmatchingpid.h"
#include "submodulesdialog.h"

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

protected:
    void resizeEvent(QResizeEvent * event);

private:
    void resizeEmbeds();

    Ui::MainWindow *ui;
    WindowsMatchingPid _WMP;
    Submodules::SubmodulesDialog * _Submodules;
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
