#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->menubar->hide();

    // Run external tool checker
    _Submodules = new Submodules::SubmodulesDialog;
    connect(_Submodules, &Submodules::SubmodulesDialog::submodulesFinished, this, &MainWindow::initialize);

    _Submodules->initialize();
}

// Kill StreamLink and Chatterino on exit
MainWindow::~MainWindow()
{
    _pStreamlinkProcess->terminate();
    _pChatterinoProcess->terminate();
    delete ui;
}

// Run until Chatterino is launched
// Get winID from Chatterino Process
// Embed Chatterino into App
// Monitor /tmp for chat changes (if patched)
void MainWindow::setupChatterinoEmbed()
{
    _chatterinoUUID.clear();
    QTimer * findChatterino = new QTimer;
    connect(findChatterino, &QTimer::timeout, this, [=]()
    {
        Window result = _WMP.getWID(_pChatterinoProcess->processId());
        if(result != 0)
        {
            QWindow *window = QWindow::fromWinId(result);
            _chatContainer = createWindowContainer(window);
            _chatContainer->setParent(ui->widget);
            _chatContainer->setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Expanding);
            _chatContainer->show();

            resizeEmbeds();

            _bChatterinoEmbedded = true;
            _tChatChannelMonitor->start(500);
            findChatterino->deleteLater();
        }
    });
    findChatterino->start(10);
}

// Requires patch to chatterino to monitor channel chages
// Would use QFileSystemWatcher but it is bugged for linux
void MainWindow::chatterinoMonitor()
{
    QFile channel("/tmp/chatterino_chan");
    if(channel.open(QFile::ReadOnly))
    {
        // sChans
        //      @Arg1: New Channel
        //      @Arg2: Unique UUID for each program (if running multiple)
        QList<QByteArray> sChans = channel.readAll().split(':');

        // Invalid communication
        if(sChans.size() < 2)
        {
            db "Invalid Coms - " << sChans;
            channel.remove();
            return;
        }

        // Initiate and save Chatterino UUID unique to this client
        if(_chatterinoUUID.isEmpty())
            _chatterinoUUID = sChans[1];

        channel.close();
        // Communication is to this instant of streamlinkerino
        if(sChans[1] == _chatterinoUUID)
        {
            // Show Settings Dialog button clicked in chatterino
            // Open settings here too
            if(sChans[0] == "settings-showdialog")
            {
                QTimer::singleShot(250, _Submodules, &Submodules::SubmodulesDialog::showDialog);
            }
            // Otherwise a channel change was requested
            else if (sChans[0] != _cChatChannel)
            {
                _pStreamlinkProcess->terminate();
                _mpvContainer->setParent(NULL);
                _cChatChannel = sChans[0];
                ui->statusbar->show();

                // Wait for current streamlink to die before restarting
                QTimer * restart = new QTimer(this);
                connect(restart, &QTimer::timeout, this, [=]()
                {
                    if(_pStreamlinkProcess->state() == 0)
                    {
                        _pStreamlinkProcess->setArguments(_Submodules->getStreamLinkArguments(_cChatChannel, _mpvContainerWID));
                        _pStreamlinkProcess->start();
                        restart->deleteLater();
                    }
                });
                restart->start(10);
            }
        }
        // Delete file to avoid duplicated commands
        // Also, ironically, allows duplicated commands to come though
        // I.e. if settings in chatterino is clicked twice
        channel.remove();
    }
}

// Read streamlink output
// show on QPlainTextEdit debug
// Update Statusbar
// Load MPV when ready
void MainWindow::readStreamLink()
{
    QString s = _pStreamlinkProcess->readAll();
    ui->plainTextEdit->setPlainText(ui->plainTextEdit->toPlainText().append(s).append("\n"));
    ui->statusbar->showMessage(s);
    if(s.contains("player: mpv"))
    {
        _mpvContainer->setParent(ui->widget_2);
        _mpvContainer->show();
        ui->statusbar->hide();
        resizeEmbeds();
    }
}

void MainWindow::initialize()
{
    // If we are re-initializing
    if(_bChatterinoEmbedded)
    {
        // Tools changed, re-initialize only what's needed
        if(_Submodules->getChanges() > 0)
        {
            if(_Submodules->getChanges() & Submodules::ChangeFlags::Chatterino)
            {
                _pChatterinoProcess->terminate();
                QTimer * restart = new QTimer(this);
                connect(restart, &QTimer::timeout, this, [=]()
                {
                    if(_pChatterinoProcess->state() == 0)
                    {
                        restart->deleteLater();
                        _pChatterinoProcess->setProgram(_Submodules->chatterinoPath());
                        _pChatterinoProcess->start();
                        setupChatterinoEmbed();
                    }
                });
                restart->start(10);
            }
            if(_Submodules->getChanges() & Submodules::ChangeFlags::StreamLink)
            {
                _mpvContainer->setParent(NULL);
                ui->statusbar->show();
                _pStreamlinkProcess->terminate();
                // Wait for current streamlink to die before restarting
                QTimer * restart = new QTimer(this);
                connect(restart, &QTimer::timeout, this, [=]()
                {
                    if(_pStreamlinkProcess->state() == 0)
                    {

                        _pStreamlinkProcess->setProgram(_Submodules->streamlinkPath());
                        _pStreamlinkProcess->setArguments(_Submodules->getStreamLinkArguments(_cChatChannel, _mpvContainerWID));
                        _pStreamlinkProcess->start();
                        restart->deleteLater();
                    }
                });
                restart->start(10);
            }
        }
        return;
    }
    // Timer to check for chat channel switches
    _tChatChannelMonitor = new QTimer;
    connect(_tChatChannelMonitor, &QTimer::timeout, this, &MainWindow::chatterinoMonitor);

    // Setup StreamLink and Chatterino Processes
    _pChatterinoProcess = new QProcess(ui->widget);
    _pChatterinoProcess->setProgram(_Submodules->chatterinoPath());
    _pChatterinoProcess->start();

    _pStreamlinkProcess = new QProcess(ui->widget_2);
    _pStreamlinkProcess->setProgram(_Submodules->streamlinkPath());
    connect(_pStreamlinkProcess, &QProcess::readyRead, this, &MainWindow::readStreamLink);

    // Setup mpv container and wid
    QWindow * mpv_window = new QWindow;
    _mpvContainerWID = mpv_window->winId();
    _mpvContainer = createWindowContainer(mpv_window);
    _mpvContainer->setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Expanding);
    _mpvContainer->hide();

    // Settings
    connect(ui->actionSettings, &QAction::triggered, _Submodules, &Submodules::SubmodulesDialog::showDialog);

    // Run Chatterino and Embed it
    setupChatterinoEmbed();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if(_bChatterinoEmbedded)
        resizeEmbeds();
    event->accept();
}

void MainWindow::resizeEmbeds()
{
    // Delay needed for X
    QTimer::singleShot(10, this, [=]()
    {
        _chatContainer->setGeometry(0,0,ui->widget->geometry().width(), ui->widget->geometry().height());
        _mpvContainer->setGeometry(0,0,ui->widget_2->geometry().width(), ui->widget_2->geometry().height());
    });
}

